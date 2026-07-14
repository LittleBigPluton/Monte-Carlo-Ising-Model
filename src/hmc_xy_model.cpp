#include "hmc_xy_model.hpp"

#include <cmath>
#include <iostream>
#include <random>
#include <vector>

void HMCXYModel::initialize_xy_spins(std::mt19937& generator){
  xy_spins.clear();
  xy_spins.reserve(grid_points);

  const double two_pi = 2.0*std::acos(-1.0);
  std::uniform_real_distribution<double> random_angle(0.0,two_pi);

  for(int i=0;i<grid_points;i++){
    xy_spins.push_back(random_angle(generator));
  }
}

double HMCXYModel::total_energy(const std::vector<double>& spin_angles) const{
  //Calculate XY-model energy without double-counting nearest-neighbour bonds
  //H = -J * sum_<ij> cos(theta_i - theta_j), with J = 1
  double total_energy_value = 0.0;

  for(int site=0;site<grid_points;site++){
    total_energy_value -= std::cos(spin_angles[site]-spin_angles[neighbours[site][0]])
                        + std::cos(spin_angles[site]-spin_angles[neighbours[site][1]]);
  }

  //This is total energy, not energy density
  return total_energy_value;
}

double HMCXYModel::total_magnetization(const std::vector<double>& spin_angles) const{
  double magnetization_x = 0.0;
  double magnetization_y = 0.0;

  for(int site=0;site<grid_points;site++){
    magnetization_x += std::cos(spin_angles[site]);
    magnetization_y += std::sin(spin_angles[site]);
  }

  double total_magnetization_value = std::sqrt(magnetization_x*magnetization_x+magnetization_y*magnetization_y);

  //This is total magnetization magnitude, not magnetization density
  return total_magnetization_value;
}

double HMCXYModel::gradient(const std::vector<double>& spin_angles,const int& site) const{
  //Derivative of the XY-model energy with respect to theta_site
  return std::sin(spin_angles[site]-spin_angles[neighbours[site][0]])
       + std::sin(spin_angles[site]-spin_angles[neighbours[site][1]])
       + std::sin(spin_angles[site]-spin_angles[neighbours[site][2]])
       + std::sin(spin_angles[site]-spin_angles[neighbours[site][3]]);
}

double HMCXYModel::kinetic_energy(const std::vector<double>& momenta) const{
  double kinetic_energy_value = 0.0;

  for(int site=0;site<grid_points;site++){
    kinetic_energy_value += 0.5*momenta[site]*momenta[site];
  }

  return kinetic_energy_value;
}

double HMCXYModel::hamiltonian(const std::vector<double>& spin_angles,const std::vector<double>& momenta,const double& beta) const{
  return kinetic_energy(momenta)+beta*total_energy(spin_angles);
}

void HMCXYModel::leapfrog(std::vector<double>& current_momenta,std::vector<double>& current_spins,const int& leapfrog_steps,const double& beta,const double& leapfrog_step_size) const{
  //First half-step update for momenta
  for(int site=0;site<grid_points;site++){
    current_momenta[site] -= 0.5*leapfrog_step_size*beta*gradient(current_spins,site);
  }

  //Full leapfrog steps
  for(int step=0;step<leapfrog_steps-1;step++){
    for(int site=0;site<grid_points;site++){
      current_spins[site] += leapfrog_step_size*current_momenta[site];
    }

    for(int site=0;site<grid_points;site++){
      current_momenta[site] -= leapfrog_step_size*beta*gradient(current_spins,site);
    }
  }

  //Final full position update
  for(int site=0;site<grid_points;site++){
    current_spins[site] += leapfrog_step_size*current_momenta[site];
  }

  //Final half-step update for momenta
  for(int site=0;site<grid_points;site++){
    current_momenta[site] -= 0.5*leapfrog_step_size*beta*gradient(current_spins,site);
  }
}

bool HMCXYModel::hmc_trajectory(const double& temperature,const int& leapfrog_steps,const double& leapfrog_step_size,std::mt19937& generator){
  if(xy_spins.empty()){
    initialize_xy_spins(generator);
  }

  double beta = 1.0/temperature;

  std::normal_distribution<double> random_momentum(0.0,1.0);
  std::uniform_real_distribution<double> uniform_random(0.0,1.0);

  std::vector<double> initial_momenta;
  initial_momenta.reserve(grid_points);

  for(int site=0;site<grid_points;site++){
    initial_momenta.push_back(random_momentum(generator));
  }

  double initial_hamiltonian = hamiltonian(xy_spins,initial_momenta,beta);

  std::vector<double> current_spins = xy_spins;
  std::vector<double> current_momenta = initial_momenta;

  leapfrog(current_momenta,current_spins,leapfrog_steps,beta,leapfrog_step_size);

  double current_hamiltonian = hamiltonian(current_spins,current_momenta,beta);
  double acceptance_probability = std::exp(-(current_hamiltonian-initial_hamiltonian));

  if(current_hamiltonian<=initial_hamiltonian || uniform_random(generator)<=acceptance_probability){
    xy_spins = current_spins;
    return true;
  }

  return false;
}

double HMCXYModel::magnetization_density() const{
  return total_magnetization(xy_spins)/grid_points;
}

double HMCXYModel::energy_density() const{
  return total_energy(xy_spins)/grid_points;
}
