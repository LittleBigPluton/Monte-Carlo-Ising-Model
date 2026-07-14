#ifndef HMC_XY_MODEL_HPP
#define HMC_XY_MODEL_HPP

#include "lattice.hpp"

#include <random>
#include <vector>

class HMCXYModel : public Lattice{
public:
  std::vector<double> xy_spins;

  void initialize_xy_spins(std::mt19937& generator);

  double total_energy(const std::vector<double>& spin_angles) const;
  double total_magnetization(const std::vector<double>& spin_angles) const;
  double gradient(const std::vector<double>& spin_angles,const int& site) const;
  double kinetic_energy(const std::vector<double>& momenta) const;
  double hamiltonian(const std::vector<double>& spin_angles,const std::vector<double>& momenta,const double& beta) const;

  void leapfrog(std::vector<double>& current_momenta,std::vector<double>& current_spins,const int& leapfrog_steps,const double& beta,const double& leapfrog_step_size) const;

  bool hmc_trajectory(const double& temperature,const int& leapfrog_steps,const double& leapfrog_step_size,std::mt19937& generator);

  double magnetization_density() const;
  double energy_density() const;
};

#endif
