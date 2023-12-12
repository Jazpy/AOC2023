#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include <unordered_map>
#include <tuple>
#include <boost/algorithm/string.hpp>

using std::cout;
using std::vector;
using std::string;
using std::unordered_map;
using std::tuple;

void get_lines(vector<string> &lines, string filename)
{
    string         line;
    std::ifstream  f(filename);

    while (std::getline(f, line))
        if (line.size() > 0)
            lines.push_back(line);
}

uint64_t validate(string &s, vector<int> &b, int hashes)
{
    if (hashes != 0)
        return 0;

    vector<int> blocks;
    int consuming = 0;

    for (auto c : s) {
        if (c == '#') {
            ++consuming;
        } else {
            blocks.push_back(consuming);
            consuming = 0;
        }
    }

    if (consuming)
        blocks.push_back(consuming);

    return blocks == b ? 1 : 0;
}

uint64_t dp(string &s, int hashes, size_t s_idx, vector<int> &b)
{
    static unordered_map<string, uint64_t> cache;
    auto   params = s + std::to_string(hashes) + "|" + std::to_string(s_idx);
    if (cache.contains(params))
        return cache[params];

    if (s_idx == s.size())
        return cache[params] = validate(s, b, hashes);

    if (s[s_idx] != '?')
        return cache[params] = dp(s, hashes, s_idx + 1, b);

    auto s_dot   = s;
    s_dot[s_idx] = '.';
    uint64_t dot_valids = dp(s_dot, hashes, s_idx + 1, b);

    uint64_t hash_valids = 0;
    if (hashes > 0) {
        auto s_hash   = s;
        s_hash[s_idx] = '#';
        hash_valids   = dp(s_hash, hashes - 1, s_idx + 1, b);
    }

    return cache[params] = dot_valids + hash_valids;
}

void solution(vector<string> &lines, int64_t &silver, int64_t &gold)
{
    gold = 0;
    for (auto &l : lines) {
        vector<string> toks;
        vector<string> str_blocks;
        vector<int> blocks;
        boost::split(toks, l, boost::is_any_of(" "), boost::token_compress_on);
        toks[0] = toks[0] + "?" + toks[0] + "?" +toks[0] + "?" + toks[0] + "?" + toks[0];
        toks[1] = toks[1] + "," + toks[1] + "," +toks[1] + "," + toks[1] + "," + toks[1];
        boost::split(str_blocks, toks[1], boost::is_any_of(","));

        int hashes = 0;
        for (auto &s : str_blocks) {
            blocks.push_back(stol(s));
            hashes += stol(s);
        }

        for (auto c : toks[0]) {
            if (c == '#')
                --hashes;
        }

        uint64_t temp = dp(toks[0], hashes, 0, blocks);
        cout << temp << std::endl;
        gold += temp;
    }
}

int main()
{
    vector<string> lines;
    get_lines(lines, "inputs/12.txt");

    int64_t silver = 0;
    int64_t gold   = 0;

    solution(lines, silver, gold);
    cout << silver << "\n" << gold << "\n";

    return 0;
}
