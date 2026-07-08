#ifndef LATTICE_HPP
#define LATTICE_HPP

#include<vector>
#include<string>

class Lattice{
protected:
  int lattice_length; //Lattice size
public:
  std::vector<int> spins; //To store spins for each point
  std::vector<std::vector<int>> neighbours; // To store nearest neighbours of each spin
  int grid_points; //Grid points
  void initialize();
  void nearest_neighbour();
};

#endif  // LATTICE_HPP
