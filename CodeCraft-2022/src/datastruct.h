#include <string>
#include <vector>
#include <unordered_set>

using namespace std;

#ifndef _DATASTRUCT_H
#define _DATASTRUCT_H

struct client_tag
{
    int id;
    string name;
    vector<unsigned> demands;
    unordered_set<struct node_tag *> accessible_nodes;
};

struct node_tag
{
    int id;
    string name;
    unsigned capacity;
    unsigned bandwidth;
    unordered_set<struct client_tag *> accessible_clients;
};

typedef struct
{
    string client_name;
    string node_name;
    unsigned amount;
} allocation;

typedef struct client_tag client;
typedef struct node_tag node;

void build_ds(vector<client> &clients, vector<node> &nodes);

#endif
