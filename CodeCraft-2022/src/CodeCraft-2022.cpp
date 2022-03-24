#include "datastruct.h"
#include "dataio.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <unordered_set>
#include <fstream>

using namespace std;

bool DEBUG = false;

// Calculate given a client and time t, sum of the accessible nodes capacity left (sum of available capacity)
int client_priority(client &client, size_t t, vector<node> &nodes)
{
    unsigned sum = 0;

    for (int node_id : client.accessible_nodes)
    {
        sum += nodes[node_id].capacity - nodes[node_id].allocated[t];
    }

    return (int)sum;
}

// at time t, allocate demands to input node as many as possible
// remember to update clients.demands[t], clients.allocations[t], and nodes.allocated[t]
void allocate_demands_to_node(int node_id, size_t t, vector<client> &clients, vector<node> &nodes)
{
    node &node = nodes[node_id];

    // prepare a pair vector for customise sorting
    vector<vector<int>> cid_w_prio;

    for (int client_id : node.accessible_clients)
    {
        if (clients[client_id].demands[t] == 0)
            continue;
        // store id, anlcs
        vector<int> pair{client_id, client_priority(clients[client_id], t, nodes)};
        cid_w_prio.push_back(pair);
    }

    // sort accessible clients by their priorities in ASC
    sort(cid_w_prio.begin(), cid_w_prio.end(), [](vector<int> &v1, vector<int> &v2)
         { return v1[1] < v2[1]; });

    // allocate demands
    for (size_t i = 0; i < cid_w_prio.size(); i++)
    {
        int client_id = cid_w_prio[i][0];
        unsigned allo_amount = min(node.capacity - node.allocated[t], clients[client_id].demands[t]);

        clients[client_id].demands[t] -= allo_amount;
        // check and initialise allocation
        unordered_map<int, unsigned> &client_allocation = clients[client_id].allocations[t];
        if (client_allocation.find(node_id) == client_allocation.end())
        {
            client_allocation[node_id] = 0;
        }
        client_allocation[node_id] += allo_amount;
        unordered_map<int, unsigned> &node_allocation = node.allocations[t];
        if (node_allocation.find(client_id) == node_allocation.end())
        {
            node_allocation[client_id] = 0;
        }
        node_allocation[client_id] += allo_amount;
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
    vector<unsigned> max_allocations(total_t);

    for (size_t t = 0; t < total_t; t++)
    {
        unsigned sum = 0;
        for (int client_id : node.accessible_clients)
        {
            sum += clients[client_id].demands[t];
        }
        max_allocations.push_back(min(sum, node.capacity));
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
                throw runtime_error("Demand not completed");
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

    write_allocations(clients, nodes);

    ifstream fstream;
    string filePath = DEBUG ? ".." : "";
    filePath += "/output/solution.txt";
    fstream.open(filePath, ios::out);
    size_t count = 0;
    string line;
    while (getline(fstream, line))
    {
        count++;
    }
    if (count != clients[0].demands.size() * clients.size())
    {
        throw runtime_error("Output file size is not correct");
    }

    return 0;
}
