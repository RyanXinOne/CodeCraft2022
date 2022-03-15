#include <iostream>
#include <unordered_set>
#include "dataio.h"

using namespace std;

bool DEBUG = true;

struct client_tag
{
    string id;
    vector<unsigned> demands;
    unordered_set<struct node_tag *> accessible_nodes;
};

struct node_tag
{
    string name;
    unsigned capacity;
    unsigned bandwidth;
    unordered_set<struct client_tag *> accessible_clients;
};

typedef struct
{
    string client_id;
    string node_name;
    unsigned amount;
} allocation;

typedef struct client_tag client;
typedef struct node_tag node;

int main()
{
    rawDemands raw_demands = read_demands();
    vector<rawNode> raw_nodes = read_nodes();
    vector<vector<unsigned>> raw_qoses = read_qos();
    unsigned QOS = read_config();

    size_t DEMANDSNUMBER = raw_demands.ids.size();
    size_t NODESNUMBER = raw_qoses.size();
    size_t CLIENTSNUMBER = raw_qoses[0].size();

    vector<client> clients;
    vector<node> nodes;
    vector<vector<allocation>> allocations;

    for (size_t i = 0; i < CLIENTSNUMBER; i++)
    {
        client client;

        client.id = raw_demands.ids[i];
        for (size_t j = 0; j < DEMANDSNUMBER; j++)
        {
            client.demands.push_back(raw_demands.data[j][i]);
        }
        clients.push_back(client);
    }

    for (size_t i = 0; i < NODESNUMBER; i++)
    {
        node temp;

        temp.name = raw_nodes[i].name;
        temp.capacity = raw_nodes[i].capacity;
        temp.bandwidth = 0;
        nodes.push_back(temp);
    }

    for (size_t nodeI = 0; nodeI < NODESNUMBER; nodeI++)
    {
        for (size_t clientI = 0; clientI < CLIENTSNUMBER; clientI++)
        {
            if (raw_qoses[nodeI][clientI] < QOS)
            {
                clients[clientI].accessible_nodes.insert(&nodes[nodeI]);
                nodes[nodeI].accessible_clients.insert(&clients[clientI]);
            }
        }
    }

    return 0;
}
