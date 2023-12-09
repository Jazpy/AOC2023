#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include <boost/algorithm/string.hpp>

using std::cout;
using std::vector;
using std::string;

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
    silver = 0;
    gold   = 0;
    for (auto &l : lines) {
        vector<string> toks;
        vector<vector<int64_t>> seqs;
        seqs.push_back(vector<int64_t>());
        boost::split(toks, l, boost::is_any_of(" "), boost::token_compress_on);
        std::transform(toks.begin(), toks.end(), std::back_inserter(seqs[0]), [](string &s) { return stoi(s); });

        size_t curr_seq_idx = 0;
        while (true) {
            bool all_zero = true;
            seqs.push_back(vector<int64_t>());
            size_t next_seq_idx = curr_seq_idx + 1;
            for (size_t i = 0; i != seqs[curr_seq_idx].size() - 1; ++i) {
                int64_t diff = seqs[curr_seq_idx][i + 1] - seqs[curr_seq_idx][i];

                if (diff != 0)
                    all_zero = false;

                seqs[next_seq_idx].push_back(diff);
            }

            if (all_zero)
                break;

            curr_seq_idx = next_seq_idx;
        }

        vector<int64_t> gold_diffs(seqs.size(), 0);
        for (int64_t i = seqs.size() - 2; i >= 0; --i) {
            seqs[i].push_back(seqs[i].back() + seqs[i + 1].back());
            gold_diffs[i] = seqs[i][0] - gold_diffs[i + 1];
        }

        silver += seqs[0].back();
        gold   += gold_diffs[0];
    }
}

int main()
{
    vector<string> lines;
    get_lines(lines, "inputs/09.txt");

    int64_t silver = 0;
    int64_t gold   = 0;

    solution(lines, silver, gold);
    cout << silver << "\n" << gold << "\n";

    return 0;
}
