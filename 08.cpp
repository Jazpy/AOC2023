#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <cstdint>

using std::cout;
using std::vector;
using std::string;
using std::array;
using std::map;
using std::pair;

void get_lines(vector<string> &lines, string filename)
{
    string         line;
    std::ifstream  f(filename);

    while (std::getline(f, line))
        if (line.size() > 0)
            lines.push_back(line);
}

void solution(vector<string> &lines, uint64_t &silver, uint64_t &gold)
{
    map<string, pair<string, string>> graph;
    vector<string> start_nodes;
    for (size_t i = 1; i != lines.size(); ++i) {
        string start =  lines[i].substr(0, 3);
        graph[start] = {lines[i].substr(7, 3), lines[i].substr(12, 3)};

        if (start[2] == 'A')
            start_nodes.push_back(start);
    }

    vector<uint64_t> cycles;
    for (auto &n : start_nodes) {
        size_t seq_idx = 0;
        uint64_t cycle = 0;
        while (n[2] != 'Z') {
            if (lines[0][seq_idx] == 'L')
                n = graph[n].first;
            else
                n = graph[n].second;

            seq_idx = (seq_idx + 1) % lines[0].size();
            ++cycle;
        }

        cycles.push_back(cycle);
    }

    gold = silver = cycles[0];
    for (size_t i = 1; i != cycles.size(); ++i)
        gold = (cycles[i] * gold) / std::__gcd(cycles[i], gold);
}

int main()
{
    vector<string> lines;
    get_lines(lines, "inputs/08.txt");

    uint64_t silver = 0;
    uint64_t gold   = 0;

    solution(lines, silver, gold);
    cout << silver << "\n" << gold << "\n";

    return 0;
}
