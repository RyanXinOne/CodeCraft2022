#include "schedule.h"
#include <algorithm>
#include <cmath>
#include <unordered_set>

// Calculate given a client and time t, given by sum of available capacity / demand
double client_priority(client &client, size_t t, vector<node> &nodes)
{
    unsigned aval_capacity = 0;

    for (int node_id : client.accessible_nodes)
    {
        aval_capacity += nodes[node_id].capacity - nodes[node_id].allocated[t];
    }

    return (double)aval_capacity / client.demands[t];
}

// at time t, allocate demands to input node as many as possible
// would update clients.demands[t], clients.allocations[t], and nodes.allocated[t]
void allocate_demands_to_node(int node_id, size_t t, vector<client> &clients, vector<node> &nodes)
{
    node &node = nodes[node_id];

    // prepare a pair vector for customise sorting
    vector<pair<int, double>> cid_w_prio; // <client id, priority>

    for (int client_id : node.accessible_clients)
    {
        if (clients[client_id].demands[t] == 0)
            continue;
        cid_w_prio.push_back(make_pair(client_id, client_priority(clients[client_id], t, nodes)));
    }

    // sort accessible clients by their priorities in ASC
    sort(cid_w_prio.begin(), cid_w_prio.end(), [](pair<int, double> &v1, pair<int, double> &v2)
         { return v1.second < v2.second; });

    // allocate demands
    for (size_t i = 0; i < cid_w_prio.size(); i++)
    {
        int client_id = cid_w_prio[i].first;
        unsigned allo_amount = min(node.capacity - node.allocated[t], clients[client_id].demands[t]);

        clients[client_id].demands[t] -= allo_amount;
        clients[client_id].allocations[t][node_id] = allo_amount;
        node.allocations[t][client_id] = allo_amount;
        node.allocated[t] += allo_amount;

        // if running out of capacity
        if (node.capacity == node.allocated[t])
            break;
    }
}

// Compute distance 95
unsigned calc_distance_95(node &node, vector<client> &clients)
{
    size_t total_t = clients[0].demands.size();
    size_t index_95 = ceil(total_t * 0.95) - 1;
    vector<unsigned> max_allocations;
    max_allocations.reserve(total_t);

    for (size_t t = 0; t < total_t; t++)
    {
        unsigned sum = 0;
        for (int client_id : node.accessible_clients)
        {
            sum += clients[client_id].demands[t];
        }
        max_allocations.push_back(min(sum + node.allocated[t], node.capacity));
    }
    nth_element(max_allocations.begin(), max_allocations.begin() + index_95, max_allocations.end());

    unsigned bandwidth_95 = max_allocations[index_95];
    unsigned distance_95 = 0;
    for (size_t i = 0; i < index_95; i++)
    {
        distance_95 += bandwidth_95 - max_allocations[i];
    }

    return distance_95;
}

// Schedule the demands of the clients to the nodes on min_95 strategy
void schedule_traffic(vector<client> &clients, vector<node> &nodes)
{
    unordered_set<int> remainingNodes;
    remainingNodes.reserve(nodes.size());
    for (size_t i = 0; i < nodes.size(); i++)
    {
        remainingNodes.insert(i);
    }

    while (!remainingNodes.empty())
    {
        // Initialize values
        int min_dis_95_i = -1;
        unsigned min_dis_95 = numeric_limits<unsigned>::max();

        // Select node with minimum 95 cost
        for (int i : remainingNodes)
        {
            unsigned cur_95_dis = calc_distance_95(nodes[i], clients);
            if (cur_95_dis < min_dis_95)
            {
                min_dis_95_i = i;
                min_dis_95 = cur_95_dis;
            }
        }

        // Allocate demands to node for each time t
        for (size_t t = 0; t < clients[0].demands.size(); t++)
        {
            allocate_demands_to_node(min_dis_95_i, t, clients, nodes);
        }

        remainingNodes.erase(min_dis_95_i);
    }
}
