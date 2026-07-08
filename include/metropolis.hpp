#ifndef METROPOLIS_HPP
#define METROPOLIS_HPP

#include<string>
#include<vector>
#include<random>
#include"measurements.hpp"
#include"ising_model.hpp"

class Metropolis{
private:
  double lookup_table[17];
public:
  void lookup(double& Temperature);
  void sweep(Ising& model, std::mt19937& generator, Measurements& measurements);
};

#endif // METROPOLIS_HPP
