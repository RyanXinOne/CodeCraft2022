#include <iostream>
#include "datastruct.h"

using namespace std;

bool DEBUG = true;

int main()
{
    vector<client> clients;
    vector<node> nodes;
    vector<vector<allocation>> allocations;

    build_ds(clients, nodes);

    return 0;
}
