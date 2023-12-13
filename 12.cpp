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

uint64_t dp(string &s, int hashes, size_t s_idx, vector<int> &b,
            unordered_map<string, uint64_t> &cache, int consuming, size_t b_idx)
{
    auto params = s.substr(s_idx) + std::to_string(hashes);
    for (size_t i = b_idx; i != b.size(); ++i)
        params += "+" + std::to_string(b[i]);

    if (cache.contains(params))
        return cache[params];

    while(s_idx < s.size() && s[s_idx] != '?') {
        if (s[s_idx] == '#') {
            ++consuming;
        } else if (consuming) {
            if (b_idx == b.size() || b[b_idx++] != consuming)
                return cache[params] = 0;
            consuming = 0;
        }

        ++s_idx;
    }

    if (s_idx == s.size()) {
        if (consuming && (b_idx == b.size() || b[b_idx++] != consuming))
            return cache[params] = 0;

        return cache[params] = b_idx == b.size() ? 1 : 0;
    }

    s[s_idx] = '.';
    uint64_t dot_valids = dp(s, hashes, s_idx, b, cache, consuming, b_idx);

    uint64_t hash_valids = 0;
    if (hashes > 0) {
        s[s_idx] = '#';
        hash_valids = dp(s, hashes - 1, s_idx, b, cache, consuming, b_idx);
    }

    s[s_idx] = '?';

    return cache[params] = dot_valids + hash_valids;
}

void solution(vector<string> &lines, int64_t &ret, bool gold)
{
    ret = 0;
    unordered_map<string, uint64_t> cache;
    for (auto &l : lines) {
        vector<string> toks;
        vector<string> str_blocks;
        vector<int> blocks;
        boost::split(toks, l, boost::is_any_of(" "), boost::token_compress_on);

        if (gold) {
            toks[0] = toks[0] + "?" + toks[0] + "?" +toks[0] + "?" + toks[0] + "?" + toks[0];
            toks[1] = toks[1] + "," + toks[1] + "," +toks[1] + "," + toks[1] + "," + toks[1];
        }

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

        ret += dp(toks[0], hashes, 0, blocks, cache, 0, 0);
    }
}

int main()
{
    vector<string> lines;
    get_lines(lines, "inputs/12.txt");

    int64_t silver = 0;
    int64_t gold   = 0;

    solution(lines, silver, false);
    solution(lines, gold,   true);
    cout << silver << "\n" << gold << "\n";

    return 0;
}
