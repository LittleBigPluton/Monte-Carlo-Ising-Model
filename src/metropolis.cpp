#include "metropolis.hpp"
#include "measurements.hpp"

#include <cmath>
#include <iostream>
#include <random>
//Prepare lookup table in order not to calcuate exponential function everytime in Metropolis
void Metropolis::lookup(double& Temperature)
{
  //ΔE can be only {-8,-4,0,+4,+8}
  //2*ΔE is needed for metropolis algorithm so range is [-16,16]
  for(int i=0;i<17;++i)lookup_table[i]=0;
  //Calculate possible ΔE values and insert zeros' table
  for(int i=-8;i<9;i+=4)lookup_table[i+8]=exp(-i/Temperature);
  std::cout<<"Lookup table was generated."<<"\n";
}//End of the look up table function

void Metropolis::sweep(Ising& model, std::mt19937& generator, Measurements& measurements)
{
  //Define distribution function. If needed, one can define is as global on ising.h file.
  std::uniform_real_distribution<double> realdist(0.0,1.0);
  //Compleating the next for loop is equal to one sweep
  for(int i(0);i<model.grid_points;i++){
    //Call related neighbours of the current spin from 2D NN vector.
    std::vector<int>& current_nn = model.neighbours[i];
    //Calculate ΔE energy change if spin is flipped, flip is not changed yet.
    int delta_energy=2*model.spins[i]*(model.spins[current_nn[0]]+model.spins[current_nn[1]]+
                                 model.spins[current_nn[2]]+model.spins[current_nn[3]]);
    //Find exponential value for this ΔE energy
    //Then, compare it with randomly choosen value between [0,1]
    if(realdist(generator)<=lookup_table[delta_energy+8]){
      model.spins[i] *= -1;
      model.Total_energy+=delta_energy;
      model.Total_magnetization+=2*model.spins[i];
    }//End of the Accept/Reject condition
  }//End of the one Monte Carlo sweep
  //Push back energies and magnetizations after one sweep
  measurements.energies.push_back(model.Total_energy);
  measurements.magnetizations.push_back(abs(model.Total_magnetization));
}//End of the Metropolis function
