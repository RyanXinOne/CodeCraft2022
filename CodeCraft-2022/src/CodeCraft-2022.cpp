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

    if (DEBUG)
    {
        cout << "clients: " << endl;
        for (size_t i = 0; i < 2; i++)
        {
            cout << clients[i].id << " " << clients[i].name << ": ";
            for (size_t j = 0; j < clients[i].demands.size(); j++)
            {
                cout << clients[i].demands[j] << " ";
            }
            cout << endl;
            for (size_t j = 0; j < clients[i].accessible_nodes.size(); j++)
            {
                cout << nodes[clients[i].accessible_nodes[j]].name << " ";
            }
            cout << endl;
        }
        cout << "nodes: " << endl;
        for (size_t i = 0; i < 2; i++)
        {
            cout << nodes[i].id << " " << nodes[i].name << ": " << nodes[i].capacity << endl;
            for (size_t j = 0; j < nodes[i].accessible_clients.size(); j++)
            {
                cout << clients[nodes[i].accessible_clients[j]].name << " ";
            }
            cout << endl;
        }
    }

    return 0;
}
