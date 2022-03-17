#include <iostream>
#include "datastruct.h"

using namespace std;

bool DEBUG = true;

// at time step t, allocate demands to input node as many as possible
// remember to update clients.demands[t], clients.allocations[t], and nodes.allocated[t]
void allocate_demands_to_node(int t, node &node)
{

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
