// Copyright 2019 Rica Radu Leonard

#ifndef GRAPH_H_
#define GRAPH_H_

#include <list>
#include <queue>
#include <string>
#include <vector>
#include "HashMap.h"

template <typename T>
class Graph {
 public:
  Graph() { numNodes = 0; }

  void addNode(T name) {
    nodesToNames.push_back(name);
    namesToNodes[name] = numNodes++;
    neighbours.push_back(std::list<int>());
  }

  void addEdge(T src, T dest) {
    int srcId = namesToNodes[src];
    int destId = namesToNodes[dest];
    neighbours[srcId].push_back(destId);
  }

  void removeEdge(T src, T dest) {
    int srcId = namesToNodes[src];
    int destId = namesToNodes[dest];
    neighbours[srcId].remove(destId);
  }

  bool hasEdge(T src, T dest) {
    int srcId = namesToNodes[src];
    int destId = namesToNodes[dest];
    for (auto it : neighbours[srcId]) {
      if (it == destId) {
        return 1;
      }
    }
    return 0;
  }

  std::vector<T> getNeighbours(T node) {
    std::vector<T> nodeNeighbours;
    int nodeId = namesToNodes[node];
    for (auto it : neighbours[nodeId]) {
      nodeNeighbours.push_back(nodesToNames[it]);
    }
    return nodeNeighbours;
  }

  std::vector<int> BFS(int src) {
    std::queue<int> Q;
    std::vector<int> dist(numNodes, -1);

    Q.push(src);
    dist[src] = 0;
    while (!Q.empty()) {
      int current = Q.front();
      Q.pop();
      for (auto next : neighbours[current]) {
        if (dist[next] == -1) {
          Q.push(next);
          dist[next] = dist[current] + 1;
        }
      }
    }
    return dist;
  }

  int getDist(T src, T dest) {
    int srcId = namesToNodes[src];
    int destId = namesToNodes[dest];
    return BFS(srcId)[destId];
  }

  void computeAllDist() {
    distances.clear();
    for (int node = 0; node < numNodes; ++node) {
      distances.push_back(BFS(node));
    }
  }

  int getComputedDist(T src, T dest) {
    int srcId = namesToNodes[src];
    int destId = namesToNodes[dest];
    return distances[srcId][destId];
  }

 private:
  int numNodes;
  std::vector<std::list<int>> neighbours;
  std::vector<std::vector<int>> distances;
  std::vector<T> nodesToNames;
  HashMap<T, int> namesToNodes;
};

#endif  // GRAPH_H_
