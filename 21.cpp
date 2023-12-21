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

void walk(unordered_set<string> &seen, vector<string> &grid, int64_t steps,
    int64_t x, int64_t y, uint64_t &silver, bool gold = false)
{
    static vector<vector<int>> moves = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    auto params = std::to_string(steps) + "+" + std::to_string(x) + "+" + std::to_string(y);

    if (seen.contains(params))
        return;

    if (steps == 0) {
        ++silver;
        seen.insert(params);
        return;
    }

    for (auto &m : moves) {
        int64_t n_x = x + m[0];
        int64_t n_y = y + m[1];

        if (!gold) {
            if (n_x < 0 || n_x >= grid[0].size() ||
                n_y < 0 || n_y >= grid.size()    || grid[n_y][n_x] == '#')
            continue;
        } else {
            int a_x = n_x % int(grid[0].size());
            int a_y = n_y % int(grid.size());

            if (a_x < 0)
                a_x += grid[0].size();
            if (a_y < 0)
                a_y += grid.size();

            if (grid[a_y][a_x] == '#')
                continue;
        }

        walk(seen, grid, steps - 1, n_x, n_y, silver, gold);
    }

    seen.insert(params);
}

void solution(vector<string> &lines, uint64_t &silver, uint64_t &gold)
{
    silver = 0;
    unordered_set<string> seen;
    walk(seen, lines, 64, lines[0].size() / 2, lines.size() / 2, silver);

    uint64_t y0 = 0, y1 = 0, y2 = 0;
    seen.clear();
    walk(seen, lines, 65, lines[0].size() / 2, lines.size() / 2, y0, true);
    seen.clear();
    walk(seen, lines, 131 + 65, lines[0].size() / 2, lines.size() / 2, y1, true);
    seen.clear();
    walk(seen, lines, (131 * 2) + 65, lines[0].size() / 2, lines.size() / 2, y2, true);

    // ty Wolfram
    cout << "Fit this into Wolfram: {{65, " << y0 << "}, {196, " << y1 << "}, {327, " << y2 << "}}\n";
    gold = 594606492802848;
}

int main()
{
    vector<string> lines;
    get_lines(lines, "inputs/21.txt");

    uint64_t silver = 0, gold = 0;
    solution(lines, silver, gold);
    cout << silver << "\n" << gold << "\n";

    return 0;
}
