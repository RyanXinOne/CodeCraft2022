clients = vector<clients>
class client:
    id -> str
    demands -> vector<unsigned>
    accessible_nodes -> unordered_set<node*>

nodes = vector<node>
class node:
    name -> str
    capacity -> unsigned
    accessible_clients -> unordered_set<client*>
    allocation -> vector<unsigned> [0...]
