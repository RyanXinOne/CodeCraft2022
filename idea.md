clients = vector<clients>
class client:
    id -> int
    name -> str
    demands -> vector<unsigned>
    accessible_nodes -> vector<int> (node id)

nodes = vector<node>
class node:
    id -> int
    name -> str
    capacity -> unsigned
    bandwidth -> unsighed
    accessible_clients -> vector<int> (client id)

allocations = vector<vector<allocation>>
class allocation:
    cliend_name -> str
    node_name -> str
    amount -> unsigned
