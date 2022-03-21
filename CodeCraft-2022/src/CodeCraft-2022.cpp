#include <iostream>
#include "datastruct.h"
#include <algorithm>
#include <unordered_set>
#include <cmath>

using namespace std;

bool DEBUG = true;

// Calculate given a client and time t, sum of the accessible nodes capacity left (ancls)
int accessible_nodes_left_capacity_sum(client &client, size_t t, vector<node> &nodes)
{
    int sum = 0;

    for (int node_id : client.accessible_nodes)
    {
        sum += nodes[node_id].capacity - nodes[node_id].allocated[t];
    }

    return sum;
}

// Customised compare method
bool compare_ancls(vector<int> &v1, vector<int> &v2)
{
    return v1[1] < v2[1];
}

// at time t, allocate demands to input node as many as possible
// remember to update clients.demands[t], clients.allocations[t], and nodes.allocated[t]
void allocate_demands_to_node(vector<client> &clients, vector<node> &nodes, size_t t, int node_id)
{
    // Prepare a 2D vector for customise sorting
    vector<vector<int>> id_and_anlcs;

    for (int client_id : nodes[node_id].accessible_clients)
    {
        // Store id, anlcs
        vector<int> temp{client_id, accessible_nodes_left_capacity_sum(clients[client_id], t, nodes)};
        id_and_anlcs.push_back(temp);
    }

    // Sort accessible clients by sum of their accessible nodes capacity left in ASC
    sort(id_and_anlcs.begin(), id_and_anlcs.end(), compare_ancls);

    // Allocate demands
    for (size_t i = 0; i < id_and_anlcs.size(); i++)
    {
        int client_id = id_and_anlcs[i][0];
        unsigned left_capacity = nodes[node_id].capacity - nodes[node_id].allocated[t];
        unsigned client_demand = clients[client_id].demands[t];

        // If all capacity needed
        if (left_capacity <= client_demand)
        {
            clients[client_id].demands[t] -= left_capacity;
            clients[client_id].allocations[t][node_id] = left_capacity;
            nodes[node_id].allocated[t] = nodes[node_id].capacity;
            break;
        }

        clients[client_id].demands[t] = 0;
        clients[client_id].allocations[t][node_id] = client_demand;
        nodes[node_id].allocated[t] += client_demand;
    }
}

// Compute the value of the 95% of the node, the cost of the node across whole time series
unsigned node_95(node &node, vector<client> &clients)
{
    int total_t = clients[0].demands.size();
    int num_95 = ceil(total_t * 0.95);
    vector<unsigned> max_allocated_list(total_t);

    for (int t = 0; t < total_t; t++)
    {
        unsigned sum = 0;
        for (client client : clients)
        {
            sum += client.demands[t];
        }
        max_allocated_list.push_back(sum);
    }

    sort(max_allocated_list.begin(), max_allocated_list.end());

    return max(node.capacity, max_allocated_list[num_95]);
}

// Schedule the demands of the clients to the nodes on min_95 strategy
void schedule_traffic(vector<client> &clients, vector<node> &nodes)
{
    unordered_set<int> remainingNodes;
    for (size_t i = 0; i < nodes.size(); i++)
    {
        remainingNodes.insert(i);
    }

    while (!remainingNodes.empty())
    {
        // Initialize values
        int min_95_i = *(remainingNodes.begin());
        unsigned min_95 = node_95(nodes[min_95_i], clients);

        // Select node with minimum 95 cost
        for (int i : remainingNodes)
        {
            unsigned cur_95 = node_95(nodes[i], clients);
            if (cur_95 < min_95)
            {
                min_95_i = i;
                min_95 = cur_95;
            }
        }

        // Allocate demands to node for each time t
        for (size_t t = 0; t < clients[0].demands.size(); t++)
        {
            allocate_demands_to_node(clients, nodes, t, min_95_i);
        }

        remainingNodes.erase(min_95_i);
        if (DEBUG)
        {
            cout << remainingNodes.size() << endl;
        }
    }

    // Need to check if all demand is allocated
    for (client client : clients)
    {
        for (size_t t = 0; t < client.demands.size(); t++)
        {
            if (client.demands[t] != 0)
            {
                if (DEBUG)
                {
                    cout << "Demand not completed for client " << client.id << " for time " << t << endl;
                }
            }
        }
    }
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
