#include "error.h"
#include <fstream>
#include <string>
#include <algorithm>

void detect_errors(vector<client> &clients, vector<client> &clients_copy, vector<node> &nodes)
{
    ifstream fstream;
    string filePath = DEBUG ? ".." : "";
    filePath += "/output/solution.txt";
    fstream.open(filePath);
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

    // Test if allocation is under accessible nodes and if all demands are allocated
    for (client client : clients)
    {
        // iterate over allocations
        for (size_t t = 0; t < client.allocations.size(); t++)
        {
            // iterate over nodes
            for (pair<int, unsigned> allocation : client.allocations[t])
            {
                if (find(client.accessible_nodes.begin(), client.accessible_nodes.end(), allocation.first) == client.accessible_nodes.end())
                {
                    throw runtime_error("QoS is not under constraints");
                }
            }
        }
    }

    // Need to check if all demand is allocated
    for (client client : clients)
    {
        for (size_t t = 0; t < client.demands.size(); t++)
        {
            if (client.demands[t] != 0)
            {
                throw runtime_error("Demand not completed");
            }
        }
    }

    // Test if allocation sum is equal to demands in each t
    for (size_t t = 0; t < clients[0].demands.size(); t++)
    {

        for (size_t i = 0; i < clients.size(); i++)
        {
            unsigned sum = 0;
            for (pair<int, unsigned> allocation : clients[i].allocations[t])
            {
                sum += allocation.second;
            }
            if (sum != clients_copy[i].demands[t])
            {
                throw runtime_error("Allocation sum is not equal to demands");
            }
        }
    }

    // Test if allocated in node bigger than capacity
    for (node node : nodes)
    {
        for (size_t t = 0; t < node.allocated.size(); t++)
        {
            if (node.allocated[t] > node.capacity)
            {
                throw runtime_error("Allocated in node bigger than capacity");
            }
        }
    }
}
