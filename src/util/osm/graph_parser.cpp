#include <iostream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "../../graph/structs/geom.h"
#include "../../graph/structs/structs.h"
#include "../matrix.h"
#include "./cycling_profile.h"
#include "./driving_profile.h"
#include "./graph_parser.h"
#include "./parser.h"
#include "./profile.h"
#include "./walking_profile.h"

//***************************************************
// utility classes
//***************************************************

struct OsmNode
{
    Coord point;
    int count;
};

struct ParseNode
{
    Coord point;
    NodeAttributes attr;
};

struct ParseEdge
{
    int64_t nodeA;
    int64_t nodeB;
    bool oneway;
    EdgeAttributes attr;
    std::vector<Coord> points;
};

//***************************************************
// handlers
//***************************************************

class InitWayHandler : public osm::IHandler
{
private:
    const IOSMDecoder& decoder;
    std::unordered_map<int64_t, OsmNode>& osm_nodes;
    int index = 0;

public:
    InitWayHandler(const IOSMDecoder& decoder, std::unordered_map<int64_t, OsmNode>& osm_nodes) : decoder(decoder), osm_nodes(osm_nodes) {}

    void node(const osm::Node& node) {}

    void way(const osm::Way& way)
    {
        if (!this->decoder.is_valid_highway(way.tags)) {
            return;
        }
        index++;
        if (index % 1000 == 0) {
            std::cout << index << std::endl;
        }
        int l = way.refs.size();
        for (int i = 0; i < l; i++) {
            int64_t ref = way.refs[i];
            if (!this->osm_nodes.contains(ref)) {
                this->osm_nodes[ref] = OsmNode{Coord(0.0, 0.0), 1};
            } else {
                this->osm_nodes[ref].count += 1;
            }
        }
        this->osm_nodes[way.refs[0]].count += 1;
        this->osm_nodes[way.refs[l - 1]].count += 1;
    }
};

class NodeHandler : public osm::IHandler
{
private:
    const IOSMDecoder& decoder;
    std::unordered_map<int64_t, OsmNode>& osm_nodes;
    std::vector<ParseNode>& nodes;
    std::unordered_map<int64_t, int>& index_mapping;
    int id;
    int index = 0;

public:
    NodeHandler(const IOSMDecoder& decoder, std::unordered_map<int64_t, OsmNode>& osm_nodes, std::vector<ParseNode>& nodes, std::unordered_map<int64_t, int>& index_mapping)
        : decoder(decoder), osm_nodes(osm_nodes), nodes(nodes), index_mapping(index_mapping), id(0)
    {}

    void node(const osm::Node& n)
    {
        if (!this->osm_nodes.contains(n.id)) {
            return;
        }
        index++;
        if (index % 1000 == 0) {
            std::cout << index << std::endl;
        }
        auto& on = this->osm_nodes[n.id];
        if (on.count > 1) {
            auto attr = this->decoder.decode_node_tags(n.tags);
            auto node = ParseNode{Coord{(float)n.lon, (float)n.lat}, attr};
            this->nodes.push_back(node);
            this->index_mapping[n.id] = this->id;
            this->id += 1;
        }
        on.point.lon = (float)n.lon;
        on.point.lat = (float)n.lat;
    }
    void way(const osm::Way& way) {}
};

class WayHandler : public osm::IHandler
{
private:
    const IOSMDecoder& decoder;
    std::vector<ParseEdge>& edges;
    std::unordered_map<int64_t, OsmNode>& osm_nodes;
    std::unordered_map<int64_t, int>& index_mapping;
    int index = 0;

public:
    WayHandler(const IOSMDecoder& decoder, std::vector<ParseEdge>& edges, std::unordered_map<int64_t, OsmNode>& osm_nodes, std::unordered_map<int64_t, int>& index_mapping)
        : decoder(decoder), edges(edges), osm_nodes(osm_nodes), index_mapping(index_mapping)
    {}

    void node(const osm::Node& n) {}
    void way(const osm::Way& w)
    {
        if (!this->decoder.is_valid_highway(w.tags)) {
            return;
        }
        index++;
        if (index % 1000 == 0) {
            std::cout << index << std::endl;
        }
        int l = w.refs.size();
        auto start = w.refs[0];
        auto end = w.refs[l - 1];
        int64_t curr;
        ParseEdge e;
        for (int i = 0; i < l; i++) {
            curr = w.refs[i];
            const auto& on = this->osm_nodes[curr];
            e.points.push_back(on.point);
            if (on.count > 1 && curr != start) {
                e.attr = this->decoder.decode_edge_tags(w.tags);
                e.oneway = this->decoder.is_oneway(w.tags);
                e.nodeA = this->index_mapping[start];
                e.nodeB = this->index_mapping[curr];
                this->edges.push_back(e);
                start = curr;
                e = ParseEdge{};
                e.points.push_back(on.point);
            }
        }
    }
};

//***************************************************
// main routine
//***************************************************

std::tuple<std::vector<Node>, std::vector<Edge>, Vector<int>, Vector<float>, Vector<int>> graph_from_osm(const std::string& file, const IOSMDecoder& decoder)
{
    std::vector<ParseNode> parse_nodes;
    std::vector<ParseEdge> parse_edges;
    std::unordered_map<int64_t, OsmNode> osm_nodes;
    std::unordered_map<int64_t, int> index_mapping;
    std::vector<std::reference_wrapper<osm::IHandler>> handlers;
    auto iwh = InitWayHandler(decoder, osm_nodes);
    handlers.push_back(iwh);
    auto nh = NodeHandler(decoder, osm_nodes, parse_nodes, index_mapping);
    handlers.push_back(nh);
    auto wh = WayHandler(decoder, parse_edges, osm_nodes, index_mapping);
    handlers.push_back(wh);
    osm::parse_osm(file, handlers);

    std::vector<Node> nodes = {};
    std::vector<Edge> edges = {};
    std::vector<EdgeAttributes> edge_attr = {};
    for (auto& pnode : parse_nodes) {
        decoder.finalize_node(pnode.attr, pnode.point);
        nodes.push_back(Node{pnode.point});
    }
    for (auto& pedge : parse_edges) {
        decoder.finalize_edge(pedge.attr, pedge.points);
        edges.push_back(Edge{(int)pedge.nodeA, (int)pedge.nodeB});
        edge_attr.push_back(pedge.attr);
        if (!pedge.oneway) {
            edges.push_back(Edge{(int)pedge.nodeB, (int)pedge.nodeA});
            edge_attr.push_back(pedge.attr);
        }
    }
    Vector<int> edge_speed(edges.size());
    Vector<float> edge_length(edges.size());
    Vector<int> edge_type(edges.size());
    for (int i = 0; i < edge_attr.size(); i++) {
        const auto& attr = edge_attr[i];
        edge_speed(i) = attr.speed;
        edge_length(i) = attr.length;
        edge_type(i) = static_cast<std::underlying_type_t<StreetType>>(attr.type);
    }
    return std::make_tuple(std::move(nodes), std::move(edges), std::move(edge_speed), std::move(edge_length), std::move(edge_type));
}

std::tuple<std::vector<Node>, std::vector<Edge>, Vector<int>, Vector<float>, Vector<int>> graph_from_osm(const std::string& file, const std::string& profile)
{
    if (profile == "driving") {
        auto decoder = DrivingDecoder();
        return graph_from_osm(file, decoder);
    } else if (profile == "walking") {
        auto decoder = WalkingDecoder();
        return graph_from_osm(file, decoder);
    } else if (profile == "cycling") {
        auto decoder = CyclingDecoder();
        return graph_from_osm(file, decoder);
    } else {
        auto decoder = DrivingDecoder();
        return graph_from_osm(file, decoder);
    }
}
