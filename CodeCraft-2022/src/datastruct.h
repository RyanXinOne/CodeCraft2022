#include "CodeCraft-2022.h"
#include <string>
#include <vector>
#include <unordered_map>

#ifndef DATASTRUCT_H
#define DATASTRUCT_H

typedef struct
{
    int id;
    string name;
    vector<unsigned> demands;
    vector<unordered_map<int, unsigned>> allocations; // <node_id, amount>
    vector<int> accessible_nodes;                     // <node_id>
} client;

typedef struct
{
    int id;
    string name;
    unsigned capacity;
    vector<unordered_map<int, unsigned>> allocations; // <client_id, amount>
    vector<unsigned> allocated;
    vector<int> accessible_clients; // <client_id>
} node;

void build_ds(vector<client> &ret_clients, vector<node> &ret_nodes);

#endif
