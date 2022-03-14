#include <iostream>
#include "dataio.h"

using namespace std;

bool DEBUG = true;

int main()
{
    demands demands = read_demands();
    vector<node> nodes = read_nodes();
    vector<vector<unsigned>> qos = read_qos();
    unsigned qos_constraint = read_config();

    cout << demands.ids.back().size() << endl;
    cout << nodes.at(1).name << " " << nodes.at(1).capacity << endl;
    cout << qos.size() << " " << qos.at(0).front() << endl;
    cout << qos_constraint << endl;

    return 0;
}
