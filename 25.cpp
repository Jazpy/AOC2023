#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <boost/algorithm/string.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/stoer_wagner_min_cut.hpp>
#include <boost/functional/hash.hpp>

using std::cout;
using std::vector;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::pair;

void get_lines(vector<string> &lines, string filename)
{
    string         line;
    std::ifstream  f(filename);

    while (std::getline(f, line))
        if (line.size() > 0)
            lines.push_back(line);
}

void solution(vector<string> &lines, uint64_t &silver)
{
    size_t id = 0;
    unordered_map<string, size_t> id_map;
    unordered_set<pair<size_t, size_t>, boost::hash<std::pair<size_t, size_t>>> edge_map;
    for (auto &l : lines) {
        vector<string> toks;
        vector<string> nodes;
        boost::split(toks, l, boost::is_any_of(" :"), boost::token_compress_on);
        for (auto &t : toks) {
            if (t.size()) {
                if (!id_map.contains(t))
                    id_map[t] = id++;
                nodes.push_back(t);
            }
        }

        for (size_t i = 1; i != nodes.size(); ++i)
            edge_map.insert({id_map[nodes[0]], id_map[nodes[i]]});
    }

    typedef boost::property<boost::edge_weight_t, int> edge_weight_prop;
    typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
        boost::no_property, edge_weight_prop> graph;

    graph g(id);
    for (auto &p : edge_map)
        boost::add_edge(p.first, p.second, edge_weight_prop(1), g);

    auto parity = boost::make_one_bit_color_map(num_vertices(g), get(boost::vertex_index, g));
    boost::stoer_wagner_min_cut(g, get(boost::edge_weight, g), boost::parity_map(parity));

    uint64_t c0 = 0, c1 = 0;
    for (size_t v = 0; v != id; ++v) {
        if (get(parity, v))
            ++c0;
        else
            ++c1;
    }

    silver = c0 * c1;
}

int main()
{
    vector<string> lines;
    get_lines(lines, "inputs/25.txt");

    uint64_t silver = 0;
    solution(lines, silver);
    cout << silver << "\n";

    return 0;
}
