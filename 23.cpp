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


int64_t longest(vector<string> &grid, int64_t x, int64_t y, unordered_set<string> visited, int64_t steps)
{
    if (x == grid[0].size() - 2 && y == grid.size() - 1)
        return steps;

    static vector<vector<int>> moves = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

    visited.insert(std::to_string(x) + "+" + std::to_string(y));
    if (grid[y][x] == 'v')
        return longest(grid, x, y + 1, visited, steps + 1);
    if (grid[y][x] == '>')
        return longest(grid, x + 1, y, visited, steps + 1);
    if (grid[y][x] == '<')
        return longest(grid, x - 1, y, visited, steps + 1);
    if (grid[y][x] == '^')
        return longest(grid, x, y - 1, visited, steps + 1);

    int64_t ret = 0;
    while (true) {
        vector<vector<int64_t>> valid_moves;
        for (auto &m : moves) {
            int64_t n_x = x + m[0];
            int64_t n_y = y + m[1];

            if (n_x < 0 || n_x >= grid[0].size() || n_y < 0 || n_y >= grid.size() ||
                grid[n_y][n_x] == '#' || visited.contains(std::to_string(n_x) + "+" + std::to_string(n_y)) ||
                (grid[n_y][n_x] == '>' && m[0] == -1) || (grid[n_y][n_x] == '<' && m[0] == 1) ||
                (grid[n_y][n_x] == 'v' && m[1] == -1) || (grid[n_y][n_x] == '^' && m[1] == 1))
                continue;
            valid_moves.push_back({n_x, n_y});
        }

        if (valid_moves.size() == 1) {
            x = valid_moves[0][0];
            y = valid_moves[0][1];
            visited.insert(std::to_string(x) + "+" + std::to_string(y));
            ++steps;

            if (x == grid[0].size() - 2 && y == grid.size() - 1)
                return steps;

            continue;
        }

        for (auto &v : valid_moves) {
            auto temp = longest(grid, v[0], v[1], visited, steps + 1);
            ret = temp > ret ? temp : ret;
        }
        break;
    }

    return ret;
}

void solution(vector<string> &lines, int64_t &silver, int64_t &gold)
{
    unordered_set<string> visited;
    silver = longest(lines, 1, 0, visited, 0);

    for (auto &l : lines) {
        for (auto &c : l) {
            if (c != '.' && c != '#')
                c = '.';
        }
    }

    visited.clear();
    gold = longest(lines, 1, 0, visited, 0);
}

int main()
{
    vector<string> lines;
    get_lines(lines, "inputs/23.txt");

    int64_t silver = 0, gold = 0;
    solution(lines, silver, gold);
    cout << silver << "\n" << gold << "\n";

    return 0;
}
