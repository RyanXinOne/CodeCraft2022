clients = vector<clients>
class client:
    id -> str
    demands -> vector<unsigned>
    accessible_nodes -> unordered_set<node*>

nodes = vector<node>
class node:
    name -> str
    capacity -> unsigned
    bandwidth -> unsighed
    accessible_clients -> unordered_set<client*>

allocations = vector<vector<allocation>>
class allocation:
    cliend_id -> str
    node_name -> str
    amount -> unsigned
