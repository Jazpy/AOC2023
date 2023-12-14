#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include <unordered_map>
#include <boost/algorithm/string.hpp>

using std::cout;
using std::vector;
using std::string;
using std::unordered_map;

void get_lines(vector<string> &lines, string filename)
{
    string         line;
    std::ifstream  f(filename);

    while (std::getline(f, line))
        if (line.size() > 0)
            lines.push_back(line);
}

void tilt_vertical(vector<string> &lines, bool up = true)
{
    int start = up ? 1            : lines.size() - 2;
    int end   = up ? lines.size() : -1;
    int lim   = up ? 0            : lines.size() - 1;
    int d     = up ? 1            : -1;
    for (size_t i = start; i != end; i += d) {
        for (size_t j = 0; j != lines[0].size(); ++j) {
            if (lines[i][j] != 'O')
                continue;

            int curr_y = i;
            while (curr_y != lim && lines[curr_y - d][j] == '.') {
                lines[curr_y][j]     = '.';
                lines[curr_y - d][j] = 'O';
                curr_y -= d;
            }
        }
    }
}

void tilt_horizontal(vector<string> &lines, bool left = true)
{
    int start = left ? 1               : lines[0].size() - 2;
    int end   = left ? lines[0].size() : -1;
    int lim   = left ? 0               : lines[0].size() - 1;
    int d     = left ? 1               : -1;
    for (auto &l : lines) {
        for (size_t i = start; i != end; i += d) {
            if (l[i] != 'O')
                continue;

            int curr_x = i;
            while (curr_x != lim && l[curr_x - d] == '.') {
                l[curr_x]     = '.';
                l[curr_x - d] = 'O';
                curr_x -= d;
            }
        }
    }
}

size_t memo(unordered_map<string, size_t> &c, vector<string> &lines, size_t i)
{
    string s = "";
    for (auto &l : lines)
        s += l;

    if (c.contains(s))
        return c[s];

    c[s] = i;
    return 0;
}

void solution(vector<string> &lines, uint64_t &silver, uint64_t &gold)
{
    unordered_map<string, size_t> cache;
    tilt_vertical(lines);
    memo(cache, lines, 0);

    silver = 0;
    for (size_t i = 0; i != lines.size(); ++i) {
        for (auto c : lines[i]) {
            if (c == 'O')
                silver += lines.size() - i;
        }
    }

    tilt_horizontal(lines);
    tilt_vertical(lines, false);
    tilt_horizontal(lines, false);

    int cycle_s = 0;
    int cycle_l = 0;
    for (size_t i = 1; i != 10000; ++i) {
        tilt_vertical(lines);
        cycle_s = memo(cache, lines, i);
        if (cycle_s) {
            cycle_l = i - cycle_s;
            break;
        }
        tilt_horizontal(lines);
        tilt_vertical(lines, false);
        tilt_horizontal(lines, false);
    }

    int k = ((1000000000 - cycle_s) / cycle_l);
    int remaining = 1000000000 - (cycle_l * k + cycle_s);

    for (size_t i = 0; i != remaining; ++i) {
        tilt_vertical(lines);
        tilt_horizontal(lines);
        tilt_vertical(lines, false);
        tilt_horizontal(lines, false);
    }

    gold = 0;
    for (size_t i = 0; i != lines.size(); ++i) {
        for (auto c : lines[i]) {
            if (c == 'O')
                gold += lines.size() - i;
        }
    }
}

int main()
{
    vector<string> lines;
    get_lines(lines, "inputs/14.txt");

    uint64_t silver = 0;
    uint64_t gold   = 0;
    solution(lines, silver, gold);
    cout << silver << "\n" << gold << "\n";

    return 0;
}
