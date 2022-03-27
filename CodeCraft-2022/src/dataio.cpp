#include "dataio.h"
#include <fstream>

vector<unsigned> parse_line_to_int(const string &line)
{
    vector<unsigned> result;
    unsigned current = 0;
    bool is_first_col = true;
    for (size_t i = 0; i < line.size(); i++)
    {
        if (line[i] == '\r')
        {
            break;
        }
        if (is_first_col)
        {
            if (line[i] == ',')
            {
                is_first_col = false;
            }
            continue;
        }
        if (line[i] == ',')
        {
            result.push_back(current);
            current = 0;
        }
        else
        {
            current = current * 10 + (line[i] - '0');
        }
    }
    result.push_back(current);
    return result;
}

vector<string> parse_line_to_string(const string &line)
{
    vector<string> result;
    string current = "";
    bool is_first_col = true;
    for (size_t i = 0; i < line.size(); i++)
    {
        if (line[i] == '\r')
        {
            break;
        }
        if (is_first_col)
        {
            if (line[i] == ',')
            {
                is_first_col = false;
            }
            continue;
        }
        if (line[i] == ',')
        {
            result.push_back(current);
            current = "";
        }
        else
        {
            current += line[i];
        }
    }
    result.push_back(current);
    return result;
}

string parse_line_get_first_col(const string &line)
{
    string result = "";
    for (size_t i = 0; i < line.size(); i++)
    {
        if (line[i] == '\r' || line[i] == ',')
        {
            break;
        }
        result += line[i];
    }
    return result;
}

rawClients read_clients()
{
    ifstream fstream;
    string line;
    rawClients raw_clients;
    string filePath = DEBUG ? ".." : "";
    filePath += "/data/demand.csv";

    fstream.open(filePath);
    getline(fstream, line);
    raw_clients.names = parse_line_to_string(line);
    while (getline(fstream, line))
    {
        raw_clients.demands.push_back(parse_line_to_int(line));
    }
    fstream.close();

    return raw_clients;
}

vector<rawNode> read_nodes()
{
    ifstream fstream;
    string line;
    vector<rawNode> raw_nodes;
    string filePath = DEBUG ? ".." : "";
    filePath += "/data/site_bandwidth.csv";

    fstream.open(filePath);
    getline(fstream, line);
    while (getline(fstream, line))
    {
        rawNode raw_node;
        raw_node.name = parse_line_get_first_col(line);
        raw_node.capacity = parse_line_to_int(line)[0];
        raw_nodes.push_back(raw_node);
    }
    fstream.close();

    return raw_nodes;
}

rawQoses read_qoses()
{
    ifstream fstream;
    string line;
    rawQoses raw_qoses;
    string filePath = DEBUG ? ".." : "";
    filePath += "/data/qos.csv";

    fstream.open(filePath);
    getline(fstream, line);
    raw_qoses.client_names = parse_line_to_string(line);
    while (getline(fstream, line))
    {
        raw_qoses.node_names.push_back(parse_line_get_first_col(line));
        raw_qoses.qoses.push_back(parse_line_to_int(line));
    }
    fstream.close();

    return raw_qoses;
}

unsigned read_config()
{
    ifstream fstream;
    string line;
    string filePath = DEBUG ? ".." : "";
    filePath += "/data/config.ini";

    fstream.open(filePath);
    getline(fstream, line);
    getline(fstream, line);
    fstream.close();

    return stoi(line.substr(line.find('=') + 1));
}

void write_allocations(const vector<client> &clients, const vector<node> &nodes)
{
    ofstream fstream;
    string filePath = DEBUG ? ".." : "";
    filePath += "/output/solution.txt";
    fstream.open(filePath);

    for (size_t t = 0; t < clients[0].demands.size(); t++)
    {
        for (size_t i = 0; i < clients.size(); i++)
        {
            fstream << clients[i].name << ":";
            size_t count = 0;
            for (unordered_map<int, unsigned>::const_iterator it = clients[i].allocations[t].begin(); it != clients[i].allocations[t].end(); it++)
            {
                fstream << "<" << nodes[it->first].name << "," << it->second << ">";
                if (count++ < clients[i].allocations[t].size() - 1)
                {
                    fstream << ",";
                }
            }
            if (t < clients[i].demands.size() - 1 || i < clients.size() - 1)
            {
                fstream << endl;
            }
        }
    }

    fstream.close();
}
