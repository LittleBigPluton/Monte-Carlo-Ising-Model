#include<iostream>
#include<vector>
#include<string>
#include<random>
#include<cmath>
#include<fstream>

#include"ising_model.hpp"
#include"statistics.hpp" //To use mean and variance functions, call statistics tools

//Construct a suitable data layout for spins with grid points
void Ising::discrete_spin_generator(std::mt19937& generator, bool randomize,const std::string& spin)
{
// Clean spin vector to prevent over write
spins.clear();
spins.reserve(grid_points);
// Define uniform distribution
std::uniform_int_distribution<int> distribution(0, 1);
//Define spin values
std::vector<int> values {-1,1};
//Initialize spin value as up, cold configuration.
int spin_value = 1;
//For hot/randomized configuration
if(randomize==true || spin == "hot"){
  for(int i{0};i<grid_points;i++)
    spins.push_back(values[distribution(generator)]);
  std::cout<<"Hot configuration spins are generated"<<"\n";
  return;}
//For cold spin configuration
if(spin == "down")spin_value=-1;
for(int i{0};i<grid_points;i++)
  spins.push_back(spin_value);
std::cout<<"Cold configuration spins are generated"<<"\n";
return;
}

//Energy function
void Ising::calculate_total_energy()
{
  Total_energy = 0;
  //Calculating energy between one spin and two other neighbours in different directions
  //In this case, left and upper neighbours' interactions are considering
  //H = -J*∑{S_i*S_j} - B*∑{S_i}
  //In this calculation, J = 1, Ferromagnetic, and B = 0, no external magnetic field.
  for(int i(0);i<grid_points;++i)Total_energy-=spins[i]*(spins[neighbours[i][0]]
                                                +spins[neighbours[i][1]]);
}

//Magnetization density function
void Ising::calculate_total_magnetization()
{
  Total_magnetization = 0;
  //Calculate total magnetization through whole grid
  //Add absolute value of each spins. It is equal to grid points at the begenning
  for(int i : spins)Total_magnetization+=i;
}
