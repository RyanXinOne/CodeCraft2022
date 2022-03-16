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
            for (unordered_set<int>::iterator it = clients[i].accessible_nodes.begin(); it != clients[i].accessible_nodes.end(); it++)
            {
                cout << nodes[*it].name << " ";
            }
            cout << endl;
        }
        cout << "nodes: " << endl;
        for (size_t i = 0; i < 2; i++)
        {
            cout << nodes[i].id << " " << nodes[i].name << ": " << nodes[i].capacity << endl;
        }
    }

    return 0;
}
