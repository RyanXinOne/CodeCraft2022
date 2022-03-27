#include "CodeCraft-2022.h"
#include "datastruct.h"
#include <string>
#include <vector>

#ifndef DATAIO_H
#define DATAIO_H

typedef struct
{
    vector<string> names;
    vector<vector<unsigned>> demands;
} rawClients;

typedef struct
{
    string name;
    unsigned capacity;
} rawNode;

typedef struct
{
    vector<string> client_names;
    vector<string> node_names;
    vector<vector<unsigned>> qoses;
} rawQoses;

rawClients read_clients();

vector<rawNode> read_nodes();

rawQoses read_qoses();

unsigned read_config();

void write_allocations(const vector<client> &clients, const vector<node> &nodes);

#endif
