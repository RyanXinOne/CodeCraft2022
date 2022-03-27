#include "optimise.h"
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <unordered_map>

typedef struct
{
    int client_id;
    int node_id;
    int timestep;
    int amount;
    unsigned old_bandwidth_95;
    unsigned new_bandwidth_95;
} allocation;

unsigned calc_bandwidth_95_inplace(vector<unsigned> &allocated)
{
    static size_t index_95 = ceil(allocated.size() * 0.95) - 1;
    nth_element(allocated.begin(), allocated.begin() + index_95, allocated.end());
    return allocated[index_95];
}

unsigned calc_bandwidth_95(const vector<unsigned> &allocated)
{
    vector<unsigned> allocated_copy = allocated;
    return calc_bandwidth_95_inplace(allocated_copy);
}

unsigned calc_bandwidth_95(const vector<unsigned> &allocated, const allocation &allocation)
{
    vector<unsigned> allocated_copy = allocated;
    allocated_copy[allocation.timestep] += allocation.amount;
    return calc_bandwidth_95_inplace(allocated_copy);
}

// apply specified migration
void migrate(const allocation &from, const allocation &to, vector<client> &clients, vector<node> &nodes)
{
    unordered_map<int, unsigned> &cf_alloc_map = clients[from.client_id].allocations[from.timestep];
    cf_alloc_map[from.node_id] += from.amount;
    if (cf_alloc_map[from.node_id] == 0)
    {
        cf_alloc_map.erase(from.node_id);
    }
    unordered_map<int, unsigned> &nf_alloc_map = nodes[from.node_id].allocations[from.timestep];
    nf_alloc_map[from.client_id] += from.amount;
    if (nf_alloc_map[from.client_id] == 0)
    {
        nf_alloc_map.erase(from.client_id);
    }
    nodes[from.node_id].allocated[from.timestep] += from.amount;

    unordered_map<int, unsigned> &ct_alloc_map = clients[to.client_id].allocations[to.timestep];
    if (ct_alloc_map.find(to.node_id) == ct_alloc_map.end())
    {
        ct_alloc_map[to.node_id] = 0;
    }
    ct_alloc_map[to.node_id] += to.amount;
    unordered_map<int, unsigned> &nt_alloc_map = nodes[to.node_id].allocations[to.timestep];
    if (nt_alloc_map.find(to.client_id) == nt_alloc_map.end())
    {
        nt_alloc_map[to.client_id] = 0;
    }
    nt_alloc_map[to.client_id] += to.amount;
    nodes[to.node_id].allocated[to.timestep] += to.amount;
}

allocation select_from(const vector<node> &nodes)
{
    double ALLOC_FRAC_MIN = 0.2;
    double ALLOC_FRAC_MAX = 0.8;

    static size_t index_95 = ceil(nodes[0].allocated.size() * 0.95) - 1;
    allocation from;
    vector<pair<unsigned, unsigned>> nid_w_allocated;
    do
    {
        // select node
        from.node_id = rand() % nodes.size();

        // find bandwidth 95
        nid_w_allocated = vector<pair<unsigned, unsigned>>();
        nid_w_allocated.reserve(nodes[from.node_id].allocated.size());
        for (size_t t = 0; t < nodes[from.node_id].allocated.size(); t++)
        {
            nid_w_allocated.push_back(make_pair(t, nodes[from.node_id].allocated[t]));
        }

        nth_element(nid_w_allocated.begin(), nid_w_allocated.begin() + index_95, nid_w_allocated.end(), [](pair<unsigned, unsigned> &a, pair<unsigned, unsigned> &b)
                    { return a.second < b.second; });
    } while (nid_w_allocated[index_95].second == 0);

    // timestep of 95% bandwidth
    from.timestep = nid_w_allocated[index_95].first;

    // select current allocation alloc_pair = <cliend id, amount>
    pair<int, unsigned> alloc_pair = *(next(begin(nodes[from.node_id].allocations[from.timestep]), rand() % nodes[from.node_id].allocations[from.timestep].size()));
    from.client_id = alloc_pair.first;

    double alloc_fraction = (rand() / (double)RAND_MAX) * (ALLOC_FRAC_MAX - ALLOC_FRAC_MIN) + ALLOC_FRAC_MIN;
    from.amount = -(int)(alloc_pair.second * alloc_fraction);

    from.old_bandwidth_95 = nid_w_allocated[index_95].second;
    from.new_bandwidth_95 = calc_bandwidth_95(nodes[from.node_id].allocated, from);

    return from;
}

allocation select_to(const allocation &from, const vector<client> &clients, const vector<node> &nodes)
{
    int best_grade = numeric_limits<int>::min();
    allocation best_to;

    allocation to;
    to.client_id = from.client_id;
    to.timestep = from.timestep;
    to.amount = -from.amount;
    for (size_t i = 0; i < clients[to.client_id].accessible_nodes.size(); i++)
    {
        to.node_id = clients[to.client_id].accessible_nodes[i];
        to.old_bandwidth_95 = calc_bandwidth_95(nodes[to.node_id].allocated);
        // check if migration is possible
        if (nodes[to.node_id].allocated[to.timestep] + to.amount > nodes[to.node_id].capacity)
            continue;
        to.new_bandwidth_95 = calc_bandwidth_95(nodes[to.node_id].allocated, to);
        // calculate improvement grade
        int grade = from.old_bandwidth_95 - from.new_bandwidth_95 + to.old_bandwidth_95 - to.new_bandwidth_95;
        if (grade > best_grade)
        {
            best_grade = grade;
            best_to = to;
        }
    }

    // indicate not possible to migrate
    if (best_grade <= 0)
    {
        best_to.node_id = -1;
    }

    return best_to;
}

// Optimize by decreasing overall bandwidth 95
void optimise_once(vector<client> &clients, vector<node> &nodes)
{

    allocation from = select_from(nodes);
    allocation to = select_to(from, clients, nodes);

    if (to.node_id > -1)
    {
        migrate(from, to, clients, nodes);
    }
}

void optimise_traffic(vector<client> &clients, vector<node> &nodes)
{
    // set num of iterations, can also set time limit
    int EPOCH = 10000;

    for (int i = 0; i < EPOCH; i++)
    {
        optimise_once(clients, nodes);
    }
}
