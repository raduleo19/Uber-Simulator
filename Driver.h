// Copyright 2019 Rica Radu Leonard

#ifndef DRIVER_H_
#define DRIVER_H_
#include <string>

class Driver {
 public:
  std::string name;
  std::string position;
  bool status;
  int rating;
  int rides;
  int dist;
  Driver(std::string name, std::string position)
      : name(name), position(position) {
    status = 1;
    rating = 0;
    rides = 0;
    dist = 0;
  }
};
#endif  // DRIVER_H_
