#include <string>
#include <vector>

using namespace std;

typedef struct
{
    vector<string> ids;
    vector<vector<unsigned>> data;
} demands;

typedef struct
{
    string name;
    unsigned capacity;
} node;


demands read_demands();

vector<node> read_nodes();

vector<vector<unsigned>> read_qos();

unsigned read_config();
