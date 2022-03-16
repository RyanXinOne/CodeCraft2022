#include "datastruct.h"
#include "dataio.h"

using namespace std;

/**
 * @brief Builds the data structures from raw data.
 */
void build_ds(vector<client> &clients, vector<node> &nodes)
{
    rawDemands raw_demands = read_demands();
    vector<rawNode> raw_nodes = read_nodes();
    vector<vector<unsigned>> raw_qoses = read_qoses();
    unsigned QOS = read_config();

    for (size_t i = 0; i < raw_qoses[0].size(); i++)
    {
        client client;
        client.id = i;
        client.name = raw_demands.ids[i];
        for (size_t j = 0; j < raw_demands.ids.size(); j++)
        {
            client.demands.push_back(raw_demands.data[j][i]);
        }
        clients.push_back(client);
    }

    for (size_t i = 0; i < raw_qoses.size(); i++)
    {
        node node;
        node.id = i;
        node.name = raw_nodes[i].name;
        node.capacity = raw_nodes[i].capacity;
        node.bandwidth = 0;
        nodes.push_back(node);
    }

    for (size_t nodeI = 0; nodeI < raw_qoses.size(); nodeI++)
    {
        for (size_t clientI = 0; clientI < raw_qoses[0].size(); clientI++)
        {
            if (raw_qoses[nodeI][clientI] < QOS)
            {
                clients[clientI].accessible_nodes.insert(&nodes[nodeI]);
                nodes[nodeI].accessible_clients.insert(&clients[clientI]);
            }
        }
    }
}
