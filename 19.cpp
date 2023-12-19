#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <boost/algorithm/string.hpp>
#include <boost/multiprecision/cpp_int.hpp>

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

class part {
    private:
    int64_t x, m, a, s, rating;
    map<char, int64_t> access_map;

    public:
    part(const string &l)
    {
        vector<string> toks;
        boost::split(toks, l, boost::is_any_of(",}"));
        x = stol(toks[0].substr(toks[0].find("=") + 1));
        m = stol(toks[1].substr(toks[1].find("=") + 1));
        a = stol(toks[2].substr(toks[2].find("=") + 1));
        s = stol(toks[3].substr(toks[3].find("=") + 1));
        rating = x + m + a + s;

        access_map = {{'x', x}, {'m', m}, {'a', a}, {'s', s}};
    }

    int64_t access(char c) const { return access_map.at(c); }
    int64_t get_rating()   const { return rating; }
};

class workflow_set {
    private:
    class operation {
        private:
        char     opt, opn;
        string   nxt;
        int64_t n;

        public:
        operation(const string &l)
        {
            if (l[1] == '<' || l[1] == '>') {
                vector<string> toks;
                boost::split(toks, l, boost::is_any_of(":"));
                opt = toks[0][1];
                opn = toks[0][0];
                n   = stol(toks[0].substr(2));
                nxt = toks[1];
            } else {
                opt = 'x';
                nxt = l;
            }
        }

        bool apply(const part &p) const
        {
            if (opt == 'x')
                return true;
            return opt == '>' ? p.access(opn) > n : p.access(opn) < n;
        }

        bool fits_constraints(map<char, vector<int64_t>> &constraints) const
        {
            if (opt == 'x')
                return nxt != "R";
            return opt == '>' ? constraints[opn][1] > n : constraints[opn][0] < n;
        }

        map<char, vector<int64_t>> fit_accept(const map<char, vector<int64_t>> &constraints) const
        {
            auto ret = constraints;

            if (opt == 'x')
                return ret;

            auto idx = opt == '>' ? 0 :  1;
            auto dir = opt == '>' ? 1 : -1;
            ret[opn][idx] = n + dir;
            return ret;
        }

        void fit_fail(map<char, vector<int64_t>> &constraints) const
        {
            auto idx = opt == '>' ?  1 : 0;
            constraints[opn][idx] = n;
        }

        bool   is_accept()   const { return opt == 'x' && nxt == "A"; }
        bool   is_reject()   const { return opt == 'x' && nxt == "R"; }
        string get_next()    const { return nxt; };
    };

    map<string, vector<operation>> workflows;

    public:
    void insert(const string &l)
    {
        vector<string> toks;
        boost::split(toks, l, boost::is_any_of("{,}"));

        vector<operation> ops;
        for (size_t i = 1; i != toks.size(); ++i)
            if (toks[i].size())
                ops.push_back(operation(toks[i]));
        workflows[toks[0]] = ops;
    }

    static void calculate_combinations(map<char, vector<int64_t>> &constraints, uint64_t &g)
    {
        int64_t total = 1;
        for (auto &p : constraints)
            total *= p.second[1] - p.second[0] + 1;
        g += total;
    }

    void explore(string w, map<char, vector<int64_t>> constraints, uint64_t &g) const
    {
        if (w == "A") {
            calculate_combinations(constraints, g);
            return;
        } else if (w == "R") {
            return;
        }

        for (const auto &op : workflows.at(w)) {
            if (op.is_reject()) {
                return;
            } if (op.is_accept()) {
                calculate_combinations(constraints, g);
            } else if (op.fits_constraints(constraints)) {
                explore(op.get_next(), op.fit_accept(constraints), g);
                op.fit_fail(constraints);
            }
        }
    }

    bool test_part(const part &p) const
    {
        string w = "in";

        while (w != "A" && w != "R") {
            const auto &ops = workflows.at(w);

            for (const auto &op : ops) {
                if (op.apply(p)) {
                    w = op.get_next();
                    break;
                }
            }
        }

        return w == "A";
    }
};

void solution(const vector<string> &lines, uint64_t &silver, uint64_t &gold)
{
    workflow_set workflows;
    vector<part> parts;
    for (auto &l : lines) {
        if (l[0] == '{')
            parts.push_back(part(l));
        else
            workflows.insert(l);
    }

    silver = 0;
    for (const auto &p : parts)
        if (workflows.test_part(p))
            silver +=  p.get_rating();

    gold = 0;
    workflows.explore("in", map<char, vector<int64_t>>({
        {'x', {1, 4000}}, {'m', {1, 4000}},
        {'a', {1, 4000}}, {'s', {1, 4000}}}), gold);
}

int main()
{
    vector<string> lines;
    get_lines(lines, "inputs/19.txt");

    uint64_t silver = 0;
    uint64_t gold   = 0;
    solution(lines, silver, gold);
    cout << silver << "\n" << gold << "\n";

    return 0;
}
