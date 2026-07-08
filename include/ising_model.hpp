#ifndef ISING_MODEL_HPP
#define ISING_MODEL_HPP

#include"lattice.hpp"
#include<string>
#include<vector>
#include<random>

class Ising: public Lattice{
protected:
  //Initialize temperature, total energy, total magnetization and length of Markov Chains
  double energy_variance, magnetization_variance;
public:
  double Total_energy, Total_magnetization;
  void discrete_spin_generator(std::mt19937& generator, bool randomize = false, const std::string& spin = "cold");
  void calculate_total_energy();
  void calculate_total_magnetization();
};

#endif  // ISING_MODEL_HPP
