#include <iostream>
#include "datastruct.h"

using namespace std;

bool DEBUG = true;

int main()
{
    vector<client> clients;
    vector<node> nodes;
    // int demands_length = clients.front().demands.size();

    build_ds(clients, nodes);



    return 0;
}
