#ifndef LATTICE_HPP
#define LATTICE_HPP

#include<vector>
#include<string>

class Lattice{
protected:
  std::vector<int> spins; //To store spins for each point
  std::vector<std::vector<int>> neighbours; // To store nearest neighbours of each spin
  int length; //Lattice size
  int grid_points; //Grid points
public:
  void initialize();
  void nearest_neighbour();
};

#endif  // LATTICE_HPP
