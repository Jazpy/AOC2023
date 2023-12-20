#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <queue>
#include <tuple>
#include <boost/algorithm/string.hpp>
#include <boost/multiprecision/cpp_int.hpp>

using std::cout;
using std::vector;
using std::string;
using std::map;
using std::unique_ptr;
using std::make_unique;
using std::queue;
using std::tuple;

void get_lines(vector<string> &lines, string filename)
{
    string         line;
    std::ifstream  f(filename);

    while (std::getline(f, line))
        if (line.size() > 0)
            lines.push_back(line);
}

class graph {
    private:
    class node {
        private:
        vector<node*>    outputs;
        map<string, int> inputs;
        char   type;
        string id;
        int    on;

        public:
        node(const string &s) : on(0)
        {
            if (s == "output") {
                type = 'o';
                id   = "output";
            }
            else if (s == "broadcaster") {
                type = 'b';
                id   = "broadcaster";
            } else {
                type = s[0];
                id   = s.substr(1);
            }
        }

        vector<tuple<int, string, string>> process_pulse(int pulse, string src)
        {
            vector<tuple<int, string, string>> ret;

            switch (type) {
                case 'o':
                    break;
                case 'b': // Same pulse to all outputs
                    for (auto p : outputs)
                        ret.push_back({pulse, p->get_id(), id});
                    return ret;
                case '%': // Flip-flop
                    if (pulse == 1)
                        return ret;
                    on = on ? 0 : 1;
                    for (auto p : outputs)
                        ret.push_back({on, p->get_id(), id});
                    return ret;
                case '&': // Conjunction
                    inputs[src] = pulse;
                    size_t join = 0;
                    for (auto &p : inputs)
                        join += p.second;
                    int o = join == inputs.size() ? 0 : 1;
                    for (auto p : outputs)
                        ret.push_back({o, p->get_id(), id});
                    return ret;
            }

            return ret;
        }

        string get_id() { return id; }
        void add_output(node *p) { outputs.push_back(p); }
        void add_input(string n)
        {
            if (type != '&')
                return;
            inputs[n] = 0;
        }
    };

    map<string, unique_ptr<node>> nodes;

    public:
    void add_nodes(const vector<string> &lines)
    {
        nodes["output"] = make_unique<node>("output");
        nodes["rx"]     = make_unique<node>("output");

        for (auto &l : lines) {
            auto source = make_unique<node>(l.substr(0, l.find(" ")));
            nodes[source->get_id()] = std::move(source);
        }

        for (auto &l : lines) {
            vector<string> toks;
            boost::split(toks, l, boost::is_any_of(" ,"), boost::token_compress_on);

            string curr_id = toks[0][0] == 'b' ? toks[0] : toks[0].substr(1);
            for (size_t i = 2; i != toks.size(); ++i) {
                if (!toks[i].size())
                    continue;
                nodes[curr_id]->add_output(nodes[toks[i]].get());
                nodes[toks[i]]->add_input(curr_id);
            }
        }
    }

    tuple<int64_t, int64_t> push_button(size_t press, map<string, uint64_t> &gold_map)
    {
        int64_t low = 1, high = 0;
        queue<tuple<int, string, string>> q;
        q.push({0, "broadcaster", "button"});

        while (!q.empty()) {
            auto [pulse, id, src] = q.front();
            q.pop();

            if ((src == "bp" || src == "xc" || src == "th" || src == "pd") && pulse == 1)
                gold_map[src] = press - gold_map[src];

            auto new_pulses = nodes[id]->process_pulse(pulse, src);
            for (auto &p : new_pulses) {
                if (std::get<0>(p) == 0)
                    ++low;
                else
                    ++high;
                q.push(p);
            }
        }

        return {low, high};
    }
};

void solution(const vector<string> &lines, uint64_t &silver, uint64_t &gold)
{
    graph g;
    g.add_nodes(lines);

    int64_t low = 0, high = 0;
    map<string, uint64_t> gold_map = {{"th", 0}, {"pd", 0}, {"xc", 0}, {"bp", 0}};
    for (size_t i = 0; i != 10000; ++i) {
        if (i == 1000)
            silver = low * high;
        auto [l, h] = g.push_button(i + 1, gold_map);
        low  += l;
        high += h;
    }
    silver = low * high;

    gold = 1;
    for (auto &p : gold_map)
        gold *= p.second;
}

int main()
{
    vector<string> lines;
    get_lines(lines, "inputs/20.txt");

    uint64_t silver = 0;
    uint64_t gold   = 0;
    solution(lines, silver, gold);
    cout << silver << "\n" << gold << "\n";

    return 0;
}
