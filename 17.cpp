#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include <queue>
#include <tuple>
#include <unordered_set>

using std::cout;
using std::vector;
using std::string;
using std::tuple;
using std::priority_queue;
using std::unordered_set;
using std::to_string;

void get_lines(vector<string> &lines, string filename)
{
    string         line;
    std::ifstream  f(filename);

    while (std::getline(f, line))
        if (line.size() > 0)
            lines.push_back(line);
}

int djikstra(vector<vector<int>> &grid, vector<vector<int>> &moves, int limit = 3)
{
    unordered_set<string> visited;
    priority_queue<tuple<int, vector<int>, vector<int>, int>,
                   vector<tuple<int, vector<int>, vector<int>, int>>,
                   std::greater<tuple<int, vector<int>, vector<int>, int>>> q;
    q.push(std::make_tuple<int, vector<int>, vector<int>, int>(0, {0, 0}, {-1, -1}, 0));

    while (!q.empty()) {
        auto [distance, curr, prev_dir, straight] = q.top();
        vector<int> prev = {curr[0] + (prev_dir[0] * -1), curr[1] + (prev_dir[1] * -1)};
        q.pop();

        string params = to_string(curr[0]) + "|" + to_string(curr[1]) + "|" +
            to_string(prev_dir[0]) + "|" + to_string(prev_dir[1]) + "|" + to_string(straight);

        if (curr[0] == grid[0].size() - 1 && curr[1] == grid.size() - 1)
            return distance;

        if (visited.contains(params))
            continue;
        visited.insert(params);

        for (auto &m : moves) {
            int next_distance = distance;
            vector<int> next  = {curr[0] + m[0], curr[1] + m[1]};
            int magnitude     = m[0] == 0 ? abs(m[1]) : abs(m[0]);
            int new_straight  = magnitude;

            // Colinear
            new_straight = magnitude;
            if ((prev[0] == curr[0] && curr[0] == next[0]) || (prev[1] == curr[1] && curr[1] == next[1]))
                new_straight = straight + magnitude;

            // Backtrack
            vector<int> inv_prev_dir = {prev_dir[0] * -1, prev_dir[1] * -1};
            if (inv_prev_dir == m)
                continue;

            // Fail
            if (next[0] < 0  || next[0] >= grid[0].size() ||
                next[1] < 0  || next[1] >= grid.size()    || new_straight > limit)
                continue;

            // Get heat loss
            if (m[0] != 0) {
                int step = m[0] / magnitude;
                for (int i = curr[0] + step; i != (next[0] + step); i += step)
                    next_distance += grid[curr[1]][i];
            } else {
                int step = m[1] / magnitude;
                for (int i = curr[1] + step; i != (next[1] + step); i += step)
                    next_distance += grid[i][next[0]];
            }

            q.push(std::make_tuple(next_distance, next, m, new_straight));
        }
    }

    return -1;
}

void solution(vector<string> &lines, int &silver, int &gold)
{
    vector<vector<int>> grid(lines.size());
    for (size_t i = 0; i != lines.size(); ++i)
        for (auto c : lines[i])
            grid[i].push_back(c - '0');

    vector<vector<int>> moves = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    silver = djikstra(grid, moves);

    moves = {{4,  0}, {-4,  0}, {0, 4},  {0, -4}, {5, 0}, {-5, 0}, {0, 5}, {0, -5},
             {6,  0}, {-6,  0}, {0, 6},  {0, -6}, {7, 0}, {-7, 0}, {0, 7}, {0, -7},
             {8,  0}, {-8,  0}, {0, 8},  {0, -8}, {9, 0}, {-9, 0}, {0, 9}, {0, -9},
             {10, 0}, {-10, 0}, {0, 10}, {0, -10}};
    gold = djikstra(grid, moves, 10);
}

int main()
{
    vector<string> lines;
    get_lines(lines, "inputs/17.txt");

    int silver = 0;
    int gold   = 0;
    solution(lines, silver, gold);
    cout << silver << "\n" << gold << "\n";

    return 0;
}
