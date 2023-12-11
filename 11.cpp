#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include <map>

using std::cout;
using std::vector;
using std::string;
using std::map;

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
    map<int64_t, vector<int64_t>> silver_galaxies;
    map<int64_t, vector<int64_t>> gold_galaxies;
    int64_t gal_idx = 0;
    int64_t y_off   = 0;
    for (int64_t y = 0; y != lines.size(); ++y) {
        if (lines[y] == string(lines[0].size(), '.')) {
            ++y_off;
            continue;
        }

        for(int64_t x = 0; x != lines[0].size(); ++x) {
            if (lines[y][x] == '#') {
                silver_galaxies[gal_idx] = {x, y + y_off};
                gold_galaxies[gal_idx++] = {x, y + (y_off * 999999)};
            }
        }
    }

    // X offsets
    int64_t x_off = 0;
    for (int64_t x = 0; x != lines[0].size(); ++x) {
        bool empty = true;
        for (int64_t y = 0; y != lines.size(); ++y) {
            if (lines[y][x] != '.') {
                empty = false;
                break;
            }
        }

        if (!empty)
            continue;

        for (auto &g : silver_galaxies) {
            if (g.second[0] > x + x_off)
                ++g.second[0];
        }

        for (auto &g : gold_galaxies) {
            if (g.second[0] > x + (x_off * 999999))
                g.second[0] += 999999;
        }

        ++x_off;
    }

    silver = 0;
    gold   = 0;
    for (int64_t a = 0; a != silver_galaxies.size() - 1; ++a) {
        for (int64_t b = a + 1; b != silver_galaxies.size(); ++b) {
            silver += std::abs(silver_galaxies[a][0] - silver_galaxies[b][0]) +
                std::abs(silver_galaxies[a][1] - silver_galaxies[b][1]);
            gold   += std::abs(gold_galaxies[a][0] - gold_galaxies[b][0]) +
                std::abs(gold_galaxies[a][1] - gold_galaxies[b][1]);
        }
    }
}

int main()
{
    vector<string> lines;
    get_lines(lines, "inputs/11.txt");

    int64_t silver = 0;
    int64_t gold   = 0;

    solution(lines, silver, gold);
    cout << silver << "\n" << gold << "\n";

    return 0;
}
