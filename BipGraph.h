#ifndef BIPARTITE_GRAPH_HPP
#define BIPARTITE_GRAPH_HPP
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>


struct IntPairHash {
    size_t operator()(const std::pair<int, int>& p) const {
        return (std::hash<int>()(p.first) ^ std::hash<int>()(p.second)) % 4294967291;
    }
};

struct PairEqual {
    bool operator()(const std::pair<int, int>& lhs, const std::pair<int, int>& rhs) const {
        return lhs.first == rhs.first && lhs.second == rhs.second;
    }
};

using myPairset = std::unordered_set<std::pair<int, int>, IntPairHash, PairEqual>;

class BipGraph {
private:
    void bfs(std::unordered_set<int>& freeVertexes);

    using myPairset = std::unordered_set<std::pair<int, int>, IntPairHash, PairEqual>;

    void dfs(int u, std::vector<bool>& visited, myPairset& augmPaths);

    void transpose();

    void symmetricDifference(myPairset& set1, myPairset& set2);
public:
    BipGraph() = default;

    BipGraph(int number, int firstR);

    void setFirstRight(int fr, int c);

    void addVertexesUntil(int number);

    void addVertex(int n);

    void addEdge(int source, int dest);

    void print() const;

    void HopcroftKarp(myPairset& answer);

    std::vector<int> getUnmatchedLeftVertexes() const;

    std::vector<int> getUnmatchedRightVertexes() const;
private:
    int firstRight;
    int maximumVertex;
    std::unordered_map<int, std::vector<int>> adjlist;
    std::vector<int> matchesLtoR;
    std::vector<int> matchesRtoL;
};

#endif //BIPARTITE_GRAPH_HPP


