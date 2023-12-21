#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_set>
#include <boost/multiprecision/cpp_int.hpp>

using std::cout;
using std::vector;
using std::string;
using std::unordered_set;

void get_lines(vector<string> &lines, string filename)
{
    string         line;
    std::ifstream  f(filename);

    while (std::getline(f, line))
        if (line.size() > 0)
            lines.push_back(line);
}

void walk_recurse(unordered_set<string> &seen, vector<string> &grid, int64_t steps,
    int64_t x, int64_t y, int64_t &silver)
{
    static vector<vector<int>> moves = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    auto params = std::to_string(steps) + "+" + std::to_string(x) + "+" + std::to_string(y);

    if (seen.contains(params))
        return;

    if (steps == 0 && grid[y][x] != 'O') {
        ++silver;
        grid[y][x] = 'O';
        seen.insert(params);
        return;
    }

    for (auto &m : moves) {
        auto n_x = x + m[0];
        auto n_y = y + m[1];

        if (n_x < 0 || n_x >= grid[0].size() || n_y < 0 || n_y >= grid.size() || grid[n_y][n_x] == '#')
            continue;

        walk_recurse(seen, grid, steps - 1, n_x, n_y, silver);
    }

    seen.insert(params);
}

void solution(vector<string> &lines, int64_t &silver, int64_t &gold)
{
    int64_t s_x, s_y;
    bool    found = false;
    for (size_t y = 0; y != lines.size() && !found; ++y) {
        for (size_t x = 0; x != lines[0].size() && !found; ++x) {
            if (lines[y][x] == 'S') {
                s_x   = x;
                s_y   = y;
                found = true;
            }
        }
    }

    silver = 0;
    unordered_set<string> seen;
    walk_recurse(seen, lines, 64, s_x, s_y, silver);
}

int main()
{
    vector<string> lines;
    get_lines(lines, "inputs/21.txt");

    int64_t silver = 0, gold = 0;
    solution(lines, silver, gold);
    cout << silver << "\n" << gold << "\n";

    return 0;
}
