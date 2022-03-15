#include <string>
#include <vector>

using namespace std;

#ifndef _DATAIO_H
#define _DATAIO_H

typedef struct
{
    vector<string> ids;
    vector<vector<unsigned>> data;
} rawDemands;

typedef struct
{
    string name;
    unsigned capacity;
} rawNode;

rawDemands read_demands();

vector<rawNode> read_nodes();

vector<vector<unsigned>> read_qoses();

unsigned read_config();

#endif
