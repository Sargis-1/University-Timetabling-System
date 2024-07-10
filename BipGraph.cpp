#include "BipGraph.h"

BipGraph::BipGraph(int number, int firstR) : firstRight(firstR), maximumVertex(number), adjlist(number), matchesRtoL(firstRight, -1), matchesLtoR(number, -1)
{}

void BipGraph::setFirstRight(int fr, int c) { 
    firstRight = fr;
    maximumVertex = c;
    matchesLtoR.resize(firstRight, -1);
    matchesRtoL.resize(c, -1);
}

void BipGraph::addVertexesUntil(int number) {
    if (number > maximumVertex)
        maximumVertex = number;

    for (int i = 0; i < number; ++i) {
        adjlist.insert({ i, {} });
    }
    matchesLtoR.resize(firstRight, -1);
    matchesRtoL.resize(number + 1, -1);
}

void BipGraph::addVertex(int n) {
    adjlist.insert({ n, {} });
    if (n > maximumVertex) {
        maximumVertex = n;
        matchesRtoL.resize(n + 1, -1);
    }
    else {
        if(n >= firstRight)
           matchesLtoR.resize(n + 1, -1);
         
    }
    
}


void BipGraph::addEdge(int source, int dest) {
    adjlist[source].push_back(dest);
}

void BipGraph::print() const {
    for (const auto& [key, value] : adjlist) {
        if (key == firstRight) {
            std::cout << std::endl;
        }
        std::cout << key << " -> ";
        for (int j = 0; j < value.size(); ++j) {
            std::cout << value[j] << " ";
        }
        std::cout << std::endl;
    }
}

void BipGraph::bfs(std::unordered_set<int>& freeVertexes)  {
    std::queue<int> q;
    std::vector<bool> visited(maximumVertex, false);
    for (const auto& [key, value] : adjlist) {//Starting from free (unmatched) left vertexes
        if (key < firstRight && matchesLtoR[key] == -1) {
            q.push(key);
        }
    }

    bool freeRVfound = false;

    while (!q.empty()) {
        if (freeRVfound) {
            break;//we stop when reached the first level of vertexes containing free right vertexes to ensure the M augmenting paths are the shortest
        }

        std::size_t size = q.size();
        while (size--) {
            int u = q.front();
            q.pop();
            visited[u] = true;
            for (int v : adjlist[u]) {
                if (!visited[v]) {
                    if (matchesRtoL[v] == -1) {
                        freeVertexes.insert(v);
                        freeRVfound = true;
                    }
                    q.push(v);
                }
            }
        }
    }

}

void BipGraph::dfs(int u, std::vector<bool>& visited, myPairset& augmPaths) {
    visited[u] = true;

    for (int v : adjlist[u]) {
        if (!visited[v]) {
            dfs(v, visited, augmPaths);
            augmPaths.insert(std::make_pair(u, v));//collecting edges which form M augmenting paths
            break;//we need only one M augmenting path
        }
    }
}

void BipGraph::transpose() {
    std::unordered_map<int, std::vector<int>> templist;
    for (const auto& [key, value] : adjlist) {
        for (int j = 0; j < value.size(); j++) {
            templist[value[j]].push_back(key);
        }
    }

    adjlist = std::move(templist);
}

void BipGraph::symmetricDifference(myPairset& set1, myPairset& set2)  {
    myPairset result;
    matchesLtoR.assign(matchesLtoR.size(), -1);//after symmetric difference some vertexes matchings will be changed
    matchesRtoL.assign(matchesRtoL.size(), -1);//after symmetric difference some vertexes matchings will be changed

    for (const auto& [a, b] : set1) {
        if (set2.find(std::make_pair(a, b)) == set2.end() && set2.find(std::make_pair(b, a)) == set2.end()) {//the edges [a, b] and [b, a] are same
            result.insert(std::make_pair(a, b));
            if (a < firstRight) {
                matchesLtoR[a] = b;
                matchesRtoL[b] = a;
            }
            else {
                matchesLtoR[b] = a;
                matchesRtoL[a] = b;
            }
        }
    }

    for (const auto& [a, b] : set2) {
        if (set1.find(std::make_pair(a, b)) == set1.end() && set1.find(std::make_pair(b, a)) == set1.end()) {
            result.insert(std::make_pair(a, b));
            if (a < firstRight) {
                matchesLtoR[a] = b;
                matchesRtoL[b] = a;
            }
            else {
                matchesLtoR[b] = a;
                matchesRtoL[a] = b;
            }
        }
    }

    set1 = std::move(result);
}


//Hopcroft-Karp algorithm finds maximum matching pairs in the Bipartite graph in O(sqrt(V) * E) time complexity
//It uses BFS to find the closest free right vertexes layer in graph, after uses DFS to find shortest M augmenting paths and makes symmetric difference with the matching set we already have
//By doing this we increase our matchings set every  reaching the maximum matching

void BipGraph::HopcroftKarp(myPairset& answer) {
    std::unordered_set<int> freeRVs;//First layer of closest free right vertexes
    myPairset matchings;//Matchings set - initially empty
    while (true) {
        freeRVs.clear();
        bfs(freeRVs);
        if (freeRVs.empty()) {//if we could reach some free right vertexes
            break;
        }

        transpose();//transposing graph to start DFS from found right vertexes
        myPairset augmentingPaths;//set to find the augmenting path edges
        std::vector<bool> visited(maximumVertex, 0);
        for (const int& u : freeRVs) {
            dfs(u, visited, augmentingPaths);//DFS from found right vertexes
        }

        symmetricDifference(matchings, augmentingPaths);//increases matchings count
        transpose();//transpose back to repeat the loop 
    }
    
    answer = std::move(matchings);
}

std::vector<int> BipGraph::getUnmatchedLeftVertexes() const
{
    std::vector<int> res;
    for (int i = 0; i < firstRight; ++i) {
        if (matchesLtoR[i] == -1)
            res.push_back(i);
    }
    return res;//NRVO opmtimization takes place (hopefully)
}

std::vector<int> BipGraph::getUnmatchedRightVertexes() const
{
    std::vector<int> res;
    for (int i = firstRight; i < matchesRtoL.size(); ++i) {
        if (matchesRtoL[i] == -1)
            res.push_back(i);
    }
    return res;
}
