#include <iostream>
#include "dataio.h"

using namespace std;

const bool DEBUG = true;

int main()
{
    rawDemands raw_demands = read_demands();
    vector<rawNode> raw_nodes = read_nodes();
    vector<vector<unsigned>> raw_qoses = read_qos();
    const unsigned QOS = read_config();

    if (DEBUG)
    {
        cout << raw_demands.ids.back().size() << endl;
        cout << raw_nodes.at(1).name << " " << raw_nodes.at(1).capacity << endl;
        cout << raw_qoses.size() << " " << raw_qoses.at(0).front() << endl;
        cout << QOS << endl;
    }

    return 0;
}
