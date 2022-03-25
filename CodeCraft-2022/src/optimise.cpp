#include "optimise.h"
#include <cstdlib>

// Choose node to be reduced
int select_reduce(vector<node> &nodes)
{
    return rand() % static_cast<int>(nodes.size());
}

// Choose node to be reduced
int select_increase(vector<node> &nodes)
{
    return rand() % static_cast<int>(nodes.size());
}

// Optimize by random node's 95 distance (changing allocations where node before receives less demand; node after receives more demand)
void optimise_traffic(vector<client> &clients, vector<node> &nodes)
{
    int reduce_node_i = select_reduce(nodes);
    int increase_node_i = select_increase(nodes);
}
