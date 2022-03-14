#include <fstream>
#include "dataio.h"
#include "CodeCradt-2022.h"

using namespace std;

vector<unsigned> parse_line_to_int(string &line, char delimiter = ',')
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
            if (line[i] == delimiter)
            {
                is_first_col = false;
            }
            continue;
        }
        if (line[i] == delimiter)
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

vector<string> parse_line_to_string(string &line, char delimiter = ',')
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
            if (line[i] == delimiter)
            {
                is_first_col = false;
            }
            continue;
        }
        if (line[i] == delimiter)
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

rawNode parse_line_to_node(string &line, char delimiter = ',')
{
    rawNode result;
    bool is_first_col = true;
    result.name = "";
    result.capacity = 0;
    for (size_t i = 0; i < line.size(); i++)
    {
        if (line[i] == '\r')
        {
            break;
        }
        if (is_first_col)
        {
            if (line[i] == delimiter)
            {
                is_first_col = false;
            }
            else
            {
                result.name += line[i];
            }
        }
        else
        {
            result.capacity = result.capacity * 10 + (line[i] - '0');
        }
    }
    return result;
}

rawDemands read_demands()
{
    ifstream fstream;
    string line;
    rawDemands rawDemands;
    string filePath = DEBUG ? ".." : "";
    filePath += "/data/demand.csv";

    fstream.open(filePath);
    getline(fstream, line);
    rawDemands.ids = parse_line_to_string(line);
    while (getline(fstream, line))
    {
        rawDemands.data.push_back(parse_line_to_int(line));
    }
    fstream.close();

    return rawDemands;
}

vector<rawNode> read_nodes()
{
    ifstream fstream;
    string line;
    vector<rawNode> rawNode;
    string filePath = DEBUG ? ".." : "";
    filePath += "/data/site_bandwidth.csv";

    fstream.open(filePath);
    getline(fstream, line);
    while (getline(fstream, line))
    {
        rawNode.push_back(parse_line_to_node(line));
    }
    fstream.close();

    return rawNode;
}

vector<vector<unsigned>> read_qos()
{
    ifstream fstream;
    string line;
    vector<vector<unsigned>> qos;
    string filePath = DEBUG ? ".." : "";
    filePath += "/data/qos.csv";

    fstream.open(filePath);
    getline(fstream, line);
    while (getline(fstream, line))
    {
        qos.push_back(parse_line_to_int(line));
    }
    fstream.close();

    return qos;
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
