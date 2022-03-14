#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

bool debug = true;

vector<int> parseLine(string line, char delimiter = ',')
{
    vector<int> result;
    int current = 0;
    for (int i = 0; i < line.size(); i++)
    {
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

int main()
{
    if (debug)
    {
        freopen("../in.txt", "r", stdin);
        freopen("../out.txt", "w", stdout);
    }

    string line;
    cin >> line;
    vector<int> numbers = parseLine(line);
    for (int i = 0; i < numbers.size(); i++)
    {
        cout << numbers[i] << ' ';
    }

    return 0;
}
