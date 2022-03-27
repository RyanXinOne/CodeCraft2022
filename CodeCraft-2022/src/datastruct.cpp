#include "datastruct.h"
#include "dataio.h"

/**
 * @brief Builds the data structures from raw data.
 */
void build_ds(vector<client> &ret_clients, vector<node> &ret_nodes)
{
    const rawClients raw_clients = read_clients();
    const vector<rawNode> raw_nodes = read_nodes();
    const rawQoses raw_qoses = read_qoses();
    const unsigned QOS = read_config();

    // stores mapping from name to client/node
    unordered_map<string, client> client_map;
    client_map.reserve(raw_clients.names.size());
    unordered_map<string, node> node_map;
    node_map.reserve(raw_nodes.size());

    for (size_t i = 0; i < raw_clients.names.size(); i++)
    {
        client client;
        client.id = i;
        client.name = raw_clients.names[i];
        client.allocations.reserve(raw_clients.demands.size());
        client.demands.reserve(raw_clients.demands.size());
        for (size_t j = 0; j < raw_clients.demands.size(); j++)
        {
            client.allocations.push_back(unordered_map<int, unsigned>());
            client.demands.push_back(raw_clients.demands[j][i]);
        }
        client_map.insert(make_pair(client.name, client));
    }

    for (size_t i = 0; i < raw_nodes.size(); i++)
    {
        node node;
        node.id = i;
        node.name = raw_nodes[i].name;
        node.capacity = raw_nodes[i].capacity;
        node.allocations.reserve(raw_clients.demands.size());
        for (size_t j = 0; j < raw_clients.demands.size(); j++)
        {
            node.allocations.push_back(unordered_map<int, unsigned>());
        }
        node.allocated = vector<unsigned>(raw_clients.demands.size());
        node_map.insert(make_pair(node.name, node));
    }

    // update accessible_clients/accessible_nodes
    for (size_t nodeI = 0; nodeI < raw_nodes.size(); nodeI++)
    {
        string node_name = raw_qoses.node_names[nodeI];
        for (size_t clientI = 0; clientI < raw_clients.names.size(); clientI++)
        {
            if (raw_qoses.qoses[nodeI][clientI] < QOS)
            {
                string client_name = raw_qoses.client_names[clientI];
                client_map[client_name].accessible_nodes.push_back(node_map[node_name].id);
                node_map[node_name].accessible_clients.push_back(client_map[client_name].id);
            }
        }
    }

    // set return values
    ret_clients = vector<client>(client_map.size());
    for (unordered_map<string, client>::iterator it = client_map.begin(); it != client_map.end(); it++)
    {
        ret_clients[it->second.id] = it->second;
    }
    ret_nodes = vector<node>(node_map.size());
    for (unordered_map<string, node>::iterator it = node_map.begin(); it != node_map.end(); it++)
    {
        ret_nodes[it->second.id] = it->second;
    }
}
