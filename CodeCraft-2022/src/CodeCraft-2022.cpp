#include "CodeCraft-2022.h"
#include "dataio.h"
#include "datastruct.h"
#include "schedule.h"
#include "optimise.h"
#include "error.h"
#include <ctime>
#include <cstdlib>
#include <vector>

bool DEBUG = true;
bool CHECKERROR = true;

int main()
{
    srand(time(NULL));

    vector<client> clients;
    vector<node> nodes;
    vector<client> clients_copy;

    build_ds(clients, nodes);

    if (CHECKERROR)
    {
        clients_copy = clients;
    }

    schedule_traffic(clients, nodes);

    optimise_traffic(clients, nodes);

    if (CHECKERROR)
    {
        detect_errors(clients, clients_copy, nodes);
    }

    write_allocations(clients, nodes);

    return 0;
}
