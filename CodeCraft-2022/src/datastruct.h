#include <string>
#include <vector>
#include <unordered_set>

using namespace std;

#ifndef _DATASTRUCT_H
#define _DATASTRUCT_H

typedef struct
{
    int id;
    string name;
    vector<unsigned> demands;
    unordered_set<int> accessible_nodes;
} client;

typedef struct
{
    int id;
    string name;
    unsigned capacity;
    unsigned bandwidth;
    unordered_set<int> accessible_clients;
} node;

typedef struct
{
    string client_name;
    string node_name;
    unsigned amount;
} allocation;

void build_ds(vector<client> &ret_clients, vector<node> &ret_nodes);

#endif
