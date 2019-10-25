// Copyright 2019 SD_Homework_Team

#ifndef SOLVER_H_
#define SOLVER_H_
#include <fstream>
#include <iomanip>
#include <queue>
#include <string>
#include <utility>
#include <vector>
#include "Driver.h"
#include "Graph.h"
#include "OrderedLinkedList.h"

class solver {
 private:
  Graph<std::string> graph;
  std::vector<Driver> drivers;
  OrderedLinkedList<double, std::string> topRating;
  OrderedLinkedList<int, std::string> topDist;
  OrderedLinkedList<int, std::string> topRides;

 public:
  void task1_solver(std::ifstream &fin, std::ofstream &fout) {
    int numNodes, numEdges;
    fin >> numNodes >> numEdges;
    for (int i = 0; i < numNodes; ++i) {
      std::string name;
      fin >> name;
      graph.addNode(name);
    }
    for (int i = 0; i < numEdges; ++i) {
      std::string source, dest;
      fin >> source >> dest;
      graph.addEdge(source, dest);
    }
    graph.computeAllDist();
    int queryNum;
    fin >> queryNum;
    for (int i = 0; i < queryNum; ++i) {
      std::string source, dest;
      fin >> source >> dest;
      if (graph.getComputedDist(source, dest) != -1) {
        fout << "y\n";
      } else {
        fout << "n\n";
      }
    }
  }

  void task2_solver(std::ifstream &fin, std::ofstream &fout) {
    int queryNum;
    fin >> queryNum;
    for (int i = 0; i < queryNum; ++i) {
      std::string source, dest;
      fin >> source >> dest;
      fout << graph.getComputedDist(source, dest) << '\n';
    }
  }

  void task3_solver(std::ifstream &fin, std::ofstream &fout) {
    int queryNum;
    fin >> queryNum;
    for (int i = 0; i < queryNum; ++i) {
      std::string command, source, dest, type;
      fin >> command >> source >> dest >> type;
      if (command == "c") {
        if (type == "0") {
          graph.addEdge(source, dest);
        } else if (type == "1") {
          graph.removeEdge(source, dest);
          graph.removeEdge(dest, source);
        } else if (type == "2") {
          graph.addEdge(dest, source);
          graph.addEdge(source, dest);
        } else if (type == "3") {
          if (graph.hasEdge(source, dest) && !graph.hasEdge(dest, source)) {
            graph.removeEdge(source, dest);
            graph.addEdge(dest, source);
          } else if (!graph.hasEdge(source, dest) &&
                     graph.hasEdge(dest, source)) {
            graph.addEdge(source, dest);
            graph.removeEdge(dest, source);
          }
        }
      } else if (command == "q") {
        if (type == "0") {
          if (graph.getDist(source, dest) != -1) {
            fout << "y\n";
          } else {
            fout << "n\n";
          }
        } else if (type == "1") {
          fout << graph.getDist(source, dest) << '\n';
        } else if (type == "2") {
          std::string via;
          fin >> via;
          int d1 = graph.getDist(source, via);
          int d2 = graph.getDist(via, dest);
          if (d1 != -1 && d2 != -1) {
            fout << d1 + d2 << '\n';
          } else {
            fout << "-1\n";
          }
        }
      }
    }
    graph.computeAllDist();
  }

  void task4_solver(std::ifstream &fin, std::ofstream &fout) {
    int queryNum;
    fin >> queryNum;
    for (int i = 0; i < queryNum; ++i) {
      std::string type;
      fin >> type;
      if (type == "d") {
        std::string name, position;
        fin >> name >> position;
        int found = 0;
        for (auto &it : drivers) {
          if (it.name == name) {
            found = 1;
            it.status = 1;
            it.position = position;
            break;
          }
        }
        if (!found) {
          drivers.push_back(Driver(name, position));
          topDist.insert(0, name);
          topRating.insert(0, name);
          topRides.insert(0, name);
        }
      } else if (type == "b") {
        std::string name;
        fin >> name;
        for (auto &it : drivers) {
          if (it.name == name) {
            it.status = 0;
            break;
          }
        }
      } else if (type == "r") {
        std::string start, end;
        int rating;
        fin >> start >> end >> rating;
        int indexMin = -1;
        for (size_t i = 0; i < drivers.size(); ++i) {
          if (drivers[i].status &&
              graph.getComputedDist(drivers[i].position, start) != -1) {
            indexMin = i;
            break;
          }
        }
        if (indexMin != -1) {
          int minDist =
              graph.getComputedDist(drivers[indexMin].position, start);
          for (size_t i = 0; i < drivers.size(); ++i) {
            int currentDist = graph.getComputedDist(drivers[i].position, start);
            if (drivers[i].status && currentDist != -1) {
              if (currentDist < minDist) {
                indexMin = i;
                minDist = currentDist;
              } else if (currentDist == minDist) {
                double currentRating, minRating;
                if (drivers[i].rides) {
                  currentRating = 1.0 * drivers[i].rating / drivers[i].rides;
                } else {
                  currentRating = 0;
                }
                if (drivers[indexMin].rides) {
                  minRating =
                      1.0 * drivers[indexMin].rating / drivers[indexMin].rides;
                } else {
                  minRating = 0;
                }
                if (currentRating > minRating) {
                  indexMin = i;
                } else if (currentRating == minRating) {
                  if (drivers[i].name < drivers[indexMin].name) {
                    indexMin = i;
                  }
                }
              }
            }
          }
          int distanceBetween = graph.getComputedDist(start, end);
          if (distanceBetween != -1) {
            drivers[indexMin].dist += minDist + distanceBetween;
            drivers[indexMin].rating += rating;
            drivers[indexMin].rides++;
            drivers[indexMin].position = end;
            topDist.update(drivers[indexMin].dist, drivers[indexMin].name);
            topRides.update(drivers[indexMin].rides, drivers[indexMin].name);
            topRating.update(
                1.0 * drivers[indexMin].rating / drivers[indexMin].rides,
                drivers[indexMin].name);
          } else {
            std::string newEnd;
            std::vector<std::string> endNeighbours = graph.getNeighbours(end);
            int found = 0;
            for (auto it : endNeighbours) {
              if (graph.getComputedDist(start, it) != -1) {
                drivers[indexMin].dist +=
                    minDist + graph.getComputedDist(start, it);
                drivers[indexMin].rating += rating;
                drivers[indexMin].rides++;
                drivers[indexMin].position = it;
                found = 1;

                topDist.update(drivers[indexMin].dist, drivers[indexMin].name);
                topRides.update(drivers[indexMin].rides,
                                drivers[indexMin].name);
                topRating.update(
                    1.0 * drivers[indexMin].rating / drivers[indexMin].rides,
                    drivers[indexMin].name);
                break;
              }
            }
            if (!found) {
              fout << "Destinatie inaccesibila\n";
            }
          }
        } else {
          fout << "Soferi indisponibili\n";
        }
      } else if (type == "top_rating") {
        int numDrivers;
        fin >> numDrivers;
        for (auto it : topRating) {
          fout << it.second << ':' << std::setprecision(3) << std::fixed
               << it.first << ' ';
          if (--numDrivers == 0) {
            break;
          }
        }
        fout << '\n';
      } else if (type == "top_dist") {
        int numDrivers;
        fin >> numDrivers;
        for (auto it : topDist) {
          fout << it.second << ':' << it.first << ' ';
          if (--numDrivers == 0) {
            break;
          }
        }
        fout << '\n';
      } else if (type == "top_rides") {
        int numDrivers;
        fin >> numDrivers;
        for (auto it : topRides) {
          fout << it.second << ':' << it.first << ' ';
          if (--numDrivers == 0) {
            break;
          }
        }
        fout << '\n';
      } else if (type == "info") {
        std::string driverName;
        fin >> driverName;
        for (auto it : drivers) {
          if (it.name == driverName) {
            double rating;
            if (it.rides == 0) {
              rating = 0;
            } else {
              rating = 1.0 * it.rating / it.rides;
            }
            std::string status;
            if (it.status) {
              status = "online";
            } else {
              status = "offline";
            }
            fout << it.name << ": " << it.position << " "
                 << std::setprecision(3) << std::fixed << rating << " "
                 << it.rides << " " << it.dist << " " << status << '\n';
            break;
          }
        }
      }
    }
  }

  void task5_solver(std::ifstream &fin, std::ofstream &fout) {
    int fuel;
    std::string name, position;
    fin >> fuel >> name;
    for (auto it : drivers) {
      if (it.name == name) {
        position = it.position;
        break;
      }
    }

    int numDest;
    fin >> numDest;
    OrderedLinkedList<int, std::string> dests;
    for (int i = 0; i < numDest; ++i) {
      std::string dest;
      fin >> dest;
      int dist = graph.getComputedDist(position, dest);
      if (dist != -1 && fuel >= dist) {
        if (!dests.hasValue(dest)) {
          dests.insert(fuel - dist, dest);
        }
      }
    }

    for (auto it : dests) {
      fout << it.second << ' ';
    }
  }
};
#endif  // SOLVER_H_
