#include <string>
#include <vector>

using namespace std;

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

vector<vector<unsigned>> read_qos();

unsigned read_config();
