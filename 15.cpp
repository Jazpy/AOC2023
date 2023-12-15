#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include <memory>
#include <boost/algorithm/string.hpp>

using std::cout;
using std::vector;
using std::string;
using std::unique_ptr;
using std::make_unique;

void get_lines(vector<string> &lines, string filename)
{
    string         line;
    std::ifstream  f(filename);

    while (std::getline(f, line))
        if (line.size() > 0)
            lines.push_back(line);
}

class linked_list {
    private:
        struct node {
            int    f;
            string label;
            unique_ptr<node> n;
            node(string label, int f, unique_ptr<node> n = nullptr) : label(label), f(f), n(std::move(n)) {}
        };

        unique_ptr<node> head;
        size_t s;

    public:
        void insert(string label, int f)
        {
            if (this->s == 0) {
                head = make_unique<node>(label, f);
            } else {
                auto curr = head.get();
                auto prev = head.get();

                while (curr) {
                    if (curr->label == label) {
                        curr->f = f;
                        return;
                    }

                    prev = curr;
                    curr = curr->n.get();
                }

                prev->n = make_unique<node>(label, f);
            }

            ++(this->s);
        }

        void remove(string label)
        {
            if (this->s == 0) {
                return;
            } else if (head->label == label) {
                this->head = std::move(this->head->n);
                --(this->s);
                return;
            }

            auto curr = head.get();
            auto prev = head.get();

            while (curr) {
                if (curr->label == label) {
                    prev->n = std::move(curr->n);
                    --(this->s);
                    return;
                }

                prev = curr;
                curr = curr->n.get();
            }
        }

        uint64_t focus_power(size_t box)
        {
            if (this->s == 0)
                return 0;

            uint64_t ret  = 0;
            uint64_t slot = 1;
            auto     curr = head.get();

            while (curr) {
                ret += curr->f * slot++ * box;
                curr = curr->n.get();
            }

            return ret;
        }
};

class hashmap {
    private:
        vector<linked_list> buckets;
    public:
        hashmap() : buckets(256) {}

        void dash(uint64_t h, string label)          { buckets[h].remove(label); }
        void equals(uint64_t h, string label, int f) { buckets[h].insert(label, f); }

        uint64_t focus_power()
        {
            uint64_t ret = 0;
            for (size_t i = 0; i != buckets.size(); ++i)
                ret += buckets[i].focus_power(i + 1);

            return ret;
        }
};

uint64_t h(string &s)
{
    uint64_t ret = 0;
    for (auto c : s)
        ret = ((ret + uint64_t(c)) * 17) % 256;
    return ret;
}

void solution(string &l, uint64_t &silver, uint64_t &gold)
{
    hashmap m;
    vector<string> toks;
    boost::split(toks, l, boost::is_any_of(","), boost::token_compress_on);

    silver = 0;
    for (auto &s : toks) {
        silver += h(s);

        vector<string> inner_toks;
        boost::split(inner_toks, s, boost::is_any_of("=-"));
        auto hash = h(inner_toks[0]);

        if (s.find('-') != string::npos)
            m.dash(hash, inner_toks[0]);
        else
            m.equals(hash, inner_toks[0], stoi(inner_toks[1]));
    }

    gold = m.focus_power();
}

int main()
{
    vector<string> lines;
    get_lines(lines, "inputs/15.txt");

    uint64_t silver = 0;
    uint64_t gold   = 0;
    solution(lines[0], silver, gold);
    cout << silver << "\n" << gold << "\n";

    return 0;
}
