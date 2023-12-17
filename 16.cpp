#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include <map>
#include <set>

using std::cout;
using std::vector;
using std::string;
using std::map;
using std::set;

void get_lines(vector<string> &lines, string filename)
{
    string         line;
    std::ifstream  f(filename);

    while (std::getline(f, line))
        if (line.size() > 0)
            lines.push_back(line);
}

void shoot_beam(vector<string> &grid, vector<vector<set<int>>> &tiles,
                int64_t x, int64_t y, int direction, uint64_t &energized)
{
    static vector<map<char, vector<int>>> bounce = {
        {{'/', {1}}, {'\\', {3}}, {'-', {0}},    {'|', {1, 3}}}, // Right
        {{'/', {0}}, {'\\', {2}}, {'-', {0, 2}}, {'|', {1}}},    // Up
        {{'/', {3}}, {'\\', {1}}, {'-', {2}},    {'|', {1, 3}}}, // Left
        {{'/', {2}}, {'\\', {0}}, {'-', {0, 2}}, {'|', {3}}}     // Down
    };
    static map<int, vector<int>> adv = {{0, {1, 0}}, {1, {0, -1}}, {2, {-1, 0}}, {3, {0, 1}}};

    if (x < 0 || x >= grid[0].size() ||
        y < 0 || y >= grid.size()    ||
        tiles[y][x].contains(direction))
        return;

    if (!tiles[y][x].size())
        ++energized;
    tiles[y][x].insert(direction);

    if (grid[y][x] == '.') {
        shoot_beam(grid, tiles, x + adv[direction][0], y + adv[direction][1], direction, energized);
        return;
    }

    for (auto d : bounce[direction][grid[y][x]])
        shoot_beam(grid, tiles, x + adv[d][0], y + adv[d][1], d, energized);
}

void clear_tiles(vector<vector<set<int>>> &tiles)
{
    for (auto &r : tiles)
        for (auto &s : r)
            s.clear();
}

void solution(vector<string> &grid, uint64_t &silver, uint64_t &gold)
{
    vector<vector<set<int>>> energized(grid.size(), vector<set<int>>(grid[0].size()));

    silver = 0;
    shoot_beam(grid, energized, 0, 0, 0, silver);

    // BROOOOOT
    gold = 0;
    for (size_t x = 0; x != grid[0].size(); ++x) {
        uint64_t candidate = 0;
        clear_tiles(energized);
        shoot_beam(grid, energized, x, 0, 3, candidate);
        gold = candidate > gold ? candidate : gold;

        candidate = 0;
        clear_tiles(energized);
        shoot_beam(grid, energized, x, grid.size() - 1, 1, candidate);
        gold = candidate > gold ? candidate : gold;
    }

    for (size_t y = 0; y != grid.size(); ++y) {
        uint64_t candidate = 0;
        clear_tiles(energized);
        shoot_beam(grid, energized, 0, y, 0, candidate);
        gold = candidate > gold ? candidate : gold;

        candidate = 0;
        clear_tiles(energized);
        shoot_beam(grid, energized, grid[0].size() - 1, y, 2, candidate);
        gold = candidate > gold ? candidate : gold;
    }
}

int main()
{
    vector<string> lines;
    get_lines(lines, "inputs/16.txt");

    uint64_t silver = 0;
    uint64_t gold   = 0;
    solution(lines, silver, gold);
    cout << silver << "\n" << gold << "\n";

    return 0;
}
