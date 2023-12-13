#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>

using std::cout;
using std::vector;
using std::string;

void get_lines(vector<string> &lines, string filename)
{
    string         line;
    std::ifstream  f(filename);

    while (std::getline(f, line))
        lines.push_back(line);
}

bool is_vertically_symmetric(string &s, int p, int &smudge)
{
    int left  = p - 1;
    int right = p;
    size_t w  = s.size();

    while (left >= 0 && right < w) {
        if (s[left] != s[right] && smudge > 0)
            --smudge;
        else if (s[left] != s[right])
            break;

        --left;
        ++right;
    }

    if (left >= 0 && right < w)
        return false;
    return true;
}

int find_vertical(vector<string> &m, int smudge = 0)
{
    for (size_t i = 1; i != m[0].size(); ++i) {
        int  curr_smudge = smudge;
        bool mirror = true;
        for (size_t j = 0; j != m.size(); ++j) {
            if (!is_vertically_symmetric(m[j], i, curr_smudge)) {
                mirror = false;
                break;
            }
        }

        if (mirror && curr_smudge == 0)
            return i;
    }

    return 0;
}

int diff_strings(string &a, string &b)
{
    int ret = 0;
    for (size_t i = 0; i != a.size(); ++i) {
        if (a[i] != b[i])
            ++ret;
    }

    return ret;
}

int find_horizontal(vector<string> &m, int smudge = 0)
{
    for (size_t i = 1; i != m.size(); ++i) {
        int curr_smudge = smudge;
        int up   = i - 1;
        int down = i;

        while (up >= 0 && down < m.size()) {
            int diffs = diff_strings(m[up], m[down]);

            if (diffs == 1 and curr_smudge)
                --curr_smudge;
            else if (diffs)
                break;

            --up;
            ++down;
        }

        if ((up < 0 || down >= m.size()) && curr_smudge == 0)
            return i * 100;
    }

    return 0;
}

void solution(vector<string> &lines, uint64_t &silver, uint64_t &gold)
{
    vector<vector<string>> maps;
    vector<string> curr_map;
    for (auto &l : lines) {
        if (l.empty()) {
            maps.push_back(curr_map);
            curr_map = vector<string>();
            continue;
        }

        curr_map.push_back(l);
    }

    if (curr_map.size())
        maps.push_back(curr_map);

    silver = 0;
    gold   = 0;
    for (auto &m : maps) {
        int v = find_vertical(m);
        if (v)
            silver += v;
        else
            silver += find_horizontal(m);

        v = find_vertical(m, 1);
        if (v)
            gold += v;
        else
            gold += find_horizontal(m, 1);
    }
}

int main()
{
    vector<string> lines;
    get_lines(lines, "inputs/13.txt");

    uint64_t silver = 0;
    uint64_t gold   = 0;
    solution(lines, silver, gold);
    cout << silver << "\n" << gold << "\n";

    return 0;
}
