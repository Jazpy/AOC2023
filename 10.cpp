#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include <map>
#include <stack>
#include <boost/algorithm/string.hpp>

using std::cout;
using std::vector;
using std::string;
using std::map;
using std::stack;

void get_lines(vector<string> &lines, string filename)
{
    string         line;
    std::ifstream  f(filename);

    while (std::getline(f, line))
        if (line.size() > 0)
            lines.push_back(line);
}

void solution(vector<string> &lines, int64_t &silver, int64_t &gold)
{
    map<char, vector<int64_t>> nav = {{'|', {0, -1, 0, 1}}, {'-', {-1, 0, 1, 0}}, {'L', {0, -1, 1, 0}},
        {'J', {0, -1, -1, 0}}, {'7', {0, 1, -1, 0}}, {'F', {0, 1, 1, 0}}};
    int64_t width  = lines[0].size();
    int64_t height = lines.size();
    vector<bool> visited(width * height, false);

    // Find start
    int64_t start_x = 0;
    int64_t start_y = 0;
    for (size_t i = 0; i != lines.size(); ++i) {
        size_t pos = lines[i].find('S');
        if (pos != string::npos) {
            start_x = pos;
            start_y = i;
            break;
        }
    }
    visited[start_y * width + start_x] = true;

    // Start neighbors
    vector<vector<int64_t>> pointers;
    for (int i = -1; i <= 1; ++i) {
        int64_t test_x = start_x + i;
        for (int j = -1; j <= 1; ++j) {
            int64_t test_y = start_y + j;
            if ((test_y < 0 || test_y >= height) ||
                (test_x < 0 || test_x >= width))
                continue;

            char c = lines[test_y][test_x];
            if (c == '.' || c == 'S')
                continue;

            auto off = nav[c];
            if ((test_x + off[0] == start_x && test_y + off[1] == start_y) ||
                (test_x + off[2] == start_x && test_y + off[3] == start_y))
                pointers.push_back({test_x, test_y});
        }
    }

    // Navigate pointers until they meet
    silver = 1;
    while (true) {
        for (auto &p : pointers) {
            visited[p[1] * width + p[0]] = true;
            auto off = nav[lines[p[1]][p[0]]];
            if (visited[(p[1] + off[1]) * width + (p[0] + off[0])]) {
                p[0] += off[2];
                p[1] += off[3];
            } else {
                p[0] += off[0];
                p[1] += off[1];
            }
        }

        ++silver;

        if (pointers[0][0] == pointers[1][0] && pointers[0][1] == pointers[1][1])
            break;
    }
    visited[pointers[0][1] * width + pointers[0][0]] = true;

    // Expand for gold squeezing
    vector<vector<char>> expanded(height * 3);
    for (auto &v : expanded)
        v = vector<char>(width * 3, '.');

    for (size_t x = 0; x != width; ++x) {
        for (size_t y = 0; y != height; ++y) {
            if (!visited[y * width + x])
                continue;

            char   c   = lines[y][x];
            size_t e_x = x * 3;
            size_t e_y = y * 3;

            expanded[e_y + 1][e_x + 1] = '#';
            if (c == 'S' || c == '-' || c == 'J' || c == '7') // W
                expanded[e_y + 1][e_x] = '#';
            if (c == 'S' || c == '|' || c == 'J' || c == 'L') // N
                expanded[e_y][e_x + 1] = '#';
            if (c == 'S' || c == '-' || c == 'F' || c == 'L') // E
                expanded[e_y + 1][e_x + 2] = '#';
            if (c == 'S' || c == '|' || c == 'F' || c == '7') // S
                expanded[e_y + 2][e_x + 1] = '#';
        }
    }

    // Paint outside component
    stack<vector<int64_t>> s;
    s.push({0, 0});

    while (!s.empty()) {
        int64_t x = s.top()[0];
        int64_t y = s.top()[1];
        s.pop();
        expanded[y][x] = 'X';

        for (int i = -1; i <= 1; ++i) {
            int64_t test_x = x + i;
            for (int j = -1; j <= 1; ++j) {
                int64_t test_y = y + j;
                if ((test_y < 0 || test_y >= expanded.size()) ||
                    (test_x < 0 || test_x >= expanded[0].size()))
                    continue;

                if (expanded[test_y][test_x] == '.')
                    s.push({test_x, test_y});
            }
        }
    }

    gold = 0;
    for (size_t x = 1; x < width * 3; x += 3) {
        for (size_t y = 1; y < height * 3; y += 3) {
            if (expanded[y][x] == '.')
                ++gold;
        }
    }
}

int main()
{
    vector<string> lines;
    get_lines(lines, "inputs/10.txt");

    int64_t silver = 0;
    int64_t gold   = 0;

    solution(lines, silver, gold);
    cout << silver << "\n" << gold << "\n";

    return 0;
}
