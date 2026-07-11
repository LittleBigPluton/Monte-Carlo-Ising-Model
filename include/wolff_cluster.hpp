#ifndef WOLFF_CLUSTER_HPP
#define WOLFF_CLUSTER_HPP

#include "ising_model.hpp"

#include <random>
#include <vector>

class WolffCluster{
public:
  void sweep(Ising& model,std::vector<double>& cluster_sizes,double& temperature,std::mt19937& generator);
};

#endif
