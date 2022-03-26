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
    unsigned amount;
} allocation;

bool try_migration(allocation from, allocation to, vector<client> &clients, vector<node> &nodes)
{
    vector<client> new_clients = clients;
    vector<node> new_nodes = nodes;

    unordered_map<int, unsigned> &cf_alloc_map = new_clients[from.client_id].allocations[from.timestep];
    cf_alloc_map[from.node_id] -= from.amount;
    if (cf_alloc_map[from.node_id] == 0)
    {
        cf_alloc_map.erase(from.node_id);
    }
    unordered_map<int, unsigned> &nf_alloc_map = new_nodes[from.node_id].allocations[from.timestep];
    nf_alloc_map[from.client_id] -= from.amount;
    if (nf_alloc_map[from.client_id] == 0)
    {
        nf_alloc_map.erase(from.client_id);
    }
    new_nodes[from.node_id].allocated[from.timestep] -= from.amount;

    unordered_map<int, unsigned> &ct_alloc_map = new_clients[to.client_id].allocations[to.timestep];
    if (ct_alloc_map.find(to.node_id) == ct_alloc_map.end())
    {
        ct_alloc_map[to.node_id] = 0;
    }
    ct_alloc_map[to.node_id] += to.amount;
    unordered_map<int, unsigned> &nt_alloc_map = new_nodes[to.node_id].allocations[to.timestep];
    if (nt_alloc_map.find(to.client_id) == nt_alloc_map.end())
    {
        nt_alloc_map[to.client_id] = 0;
    }
    nt_alloc_map[to.client_id] += to.amount;
    new_nodes[to.node_id].allocated[to.timestep] += to.amount;

    // pass - compare distance 95
}

allocation select_from(vector<node> &nodes)
{
    double ALLOC_FRAC_MIN = 0.2;
    double ALLOC_FRAC_MAX = 0.8;

    // select start node
    int start_node_i = rand() % nodes.size();
    node &node = nodes[start_node_i];

    // find bandwidth 95
    vector<pair<unsigned, unsigned>> nid_w_allocated;
    nid_w_allocated.reserve(node.allocated.size());
    for (size_t t = 0; t < node.allocated.size(); t++)
    {
        nid_w_allocated.push_back(make_pair(t, node.allocated[t]));
    }

    sort(nid_w_allocated.begin(), nid_w_allocated.end(), [](pair<unsigned, unsigned> &a, pair<unsigned, unsigned> &b)
         { return a.second < b.second; });

    size_t index_95 = ceil(node.allocated.size() * 0.95) - 1;
    size_t ts_95 = nid_w_allocated[index_95].first;

    allocation from;
    from.node_id = start_node_i;
    from.timestep = ts_95;
    pair<int, unsigned> alloc_pair = *(next(begin(node.allocations[ts_95]), rand() % node.allocations[ts_95].size()));
    double alloc_fraction = (rand() / (double)RAND_MAX) * (ALLOC_FRAC_MAX - ALLOC_FRAC_MIN) + ALLOC_FRAC_MIN;
    from.client_id = alloc_pair.first;
    from.amount = (unsigned)(alloc_pair.second * alloc_fraction);

    return from;
}

// Optimize by random node's 95 distance (changing allocations where node before receives less demand; node after receives more demand)
bool optimise_once(vector<client> &clients, vector<node> &nodes)
{
    allocation from = select_from(nodes);

    allocation to;
    to.client_id = from.client_id;
    to.timestep = from.timestep;
    to.amount = from.amount;
    for (size_t i = 0; i < clients[to.client_id].accessible_nodes.size(); i++)
    {
        to.node_id = clients[to.client_id].accessible_nodes[i];
        // check if migration is possible
        if (nodes[to.node_id].allocated[to.timestep] + to.amount <= nodes[to.node_id].capacity)
        {
            try_migration(from, to, clients, nodes);
            break;
        }
    }
}

void optimise_traffic(vector<client> &clients, vector<node> &nodes)
{
    while (true)
    {
        optimise_once(clients, nodes);
        break;
    }
}
