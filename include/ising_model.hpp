#ifndef ISING_MODEL_HPP
#define ISING_MODEL_HPP


#include"lattice.hpp"
#include<string>
#include<vector>
#include<random>

class Ising: public Lattice{
protected:
  //Initialize temperature, total energy, total magnetization and length of Markov Chains
  double aed, energy_variance, SH, amd, magnetization_variance, MS;
public:
  double Temp, Teng, Tmag;
  void discrete_spin_generator(std::mt19937& generator, bool randomize = false, const std::string& spin = "cold");
  void total_energy();
  void total_magnetization();
  void specific_heat(double variance);
  void magnetic_susceptibility(double variance);
};

#endif  // ISING_MODEL_HPP
