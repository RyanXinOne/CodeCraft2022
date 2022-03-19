#include <iostream>
#include "datastruct.h"
#include <algorithm>

using namespace std;

bool DEBUG = true;

// Calculate given a client and time t, sum of the accessible nodes capacity left (ancls)
unsigned accessible_nodes_left_capacity_sum(client &client, int t, vector<node> &nodes)
{
    unsigned sum = 0;

    for (auto node_id : client.accessible_nodes)
    {
        sum += nodes[node_id].capacity - nodes[node_id].allocated[t];
    }

    return sum;
}

// Customised compare method
bool compare_ancls(vector<int, unsigned> v1, vector<int, unsigned> v2)
{
    return v1[1] < v2[1];
}

// at time t, allocate demands to input node as many as possible
// remember to update clients.demands[t], clients.allocations[t], and nodes.allocated[t]
void allocate_demands_to_node(vector<client> &clients, vector<node> &nodes, int t, int node_id)
{
    // Prepare a 2D vector for customise sorting
    vector<vector<int, unsigned>> id_and_anlcs;

    for (auto client_id : nodes[node_id].accessible_clients)
    {

        // Store id, anlcs
        vector<int, unsigned> temp{client_id, accessible_nodes_left_capacity_sum(clients[client_id], t, nodes)};
        id_and_anlcs.push_back(temp);
    }

    // Sort accessible clients by sum of their accessible nodes capacity left in ASC
    sort(id_and_anlcs.begin(), id_and_anlcs.end(), compare_ancls);

    // Allocate demands
    for (auto i = 0; i < id_and_anlcs.size(); i++)
    {
        int client_id = id_and_anlcs[i][0];
        int left_capacity = nodes[node_id].capacity - nodes[node_id].allocated;
        int client_demand = clients[client_id].demands[t];

        // If all capacity needed
        if (left_capacity <= client_demand)
        {
            clients[client_id].demands[t] -= left_capacity;
            clients[client_id].allocations.push_back(node_id, left_capacity);
            nodes[node_id].allocated[t] = nodes[node_id].capacity;
            break;
        }

        clients[client_id].demands[t] = 0;
        clients[client_id].allocations.push_back(node_id, client_demand);
        nodes[node_id].allocated[t] += client_demand;
    }
}

void schedule_traffic(vector<client> &clients, vector<node> &nodes)
{
}

int main()
{
    vector<client> clients;
    vector<node> nodes;
    // int demands_length = clients.front().demands.size();

    build_ds(clients, nodes);

    schedule_traffic(clients, nodes);

    return 0;
}
