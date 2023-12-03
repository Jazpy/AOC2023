#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <utility>
#include <cstdint>

using std::cout;
using std::vector;
using std::string;
using std::map;
using std::pair;
using std::set;

void get_lines(vector<string> &lines, string filename)
{
    string         line;
    std::ifstream  f(filename);

    while (std::getline(f, line))
        if (line.size() > 0)
            lines.push_back(line);
}

void solution(vector<string> &matrix, uint32_t &silver, uint32_t &gold)
{
    map<pair<size_t, size_t>, vector<uint32_t>> gears;

    for (size_t y = 0; y != matrix.size(); ++y) {
        bool adj_number      = false;
        uint32_t curr_number = 0;
        set<pair<size_t, size_t>> adj_gears;

        for (size_t x = 0; x != matrix[y].size(); ++x) {
            auto c = matrix[y][x];

            // Found a number
            if (std::isdigit(c)) {
                curr_number = (curr_number * 10) + (c - '0');

                // Check surrounding symbols
                size_t left  = x == 0 ? x : x - 1;
                size_t right = x == matrix[y].size() - 1 ? x : x + 1;
                size_t up    = y == 0 ? y : y - 1;
                size_t down  = y == matrix.size() - 1 ? y : y + 1;

                for (size_t check_x = left; check_x <= right; ++check_x) {
                    for (size_t check_y = up; check_y <= down; ++check_y) {
                        auto check_c = matrix[check_y][check_x];

                        if (!std::isdigit(check_c) && check_c != '.') {
                            adj_number = true;

                            // It's a gear
                            if (check_c == '*')
                                adj_gears.insert(pair<size_t, size_t>(check_x, check_y));
                        }
                    }
                }
            } else {
                if (adj_number) {
                    silver += curr_number;

                    for (auto &g : adj_gears) {
                        if (!gears.contains(g))
                            gears[g] = vector<uint32_t>();
                        gears[g].push_back(curr_number);
                    }
                }
                if (curr_number) {
                    curr_number     = 0;
                    adj_number = false;
                    adj_gears.clear();
                }
            }
        }

        if (adj_number) {
            silver += curr_number;

            for (auto &g : adj_gears) {
                if (!gears.contains(g))
                    gears[g] = vector<uint32_t>();
                gears[g].push_back(curr_number);
            }
        }
    }

    for (auto &g : gears) {
        if (g.second.size() == 2)
            gold += g.second[0] * g.second[1];
    }
}

int main()
{
    vector<string> lines;
    get_lines(lines, "inputs/03.txt");

    uint32_t silver = 0;
    uint32_t gold   = 0;

    solution(lines, silver, gold);

    cout << silver << "\n" << gold << "\n";

    return 0;
}
