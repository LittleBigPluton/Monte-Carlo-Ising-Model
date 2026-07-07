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
//Define spin values
std::vector<int> values {-1,1};
//Initialize spin value as up, cold configuration.
int spin_value = 1;
//For hot/randomized configuration
if(randomize==true || spin == "hot"){
  for(int i{0};i<grid_points;i++)
    spins.push_back(values[rand()%2]);
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
void Ising::total_energy()
{
  Teng=0;
  //Calculating energy between one spin and two other neighbours in different directions
  //In this case, left and upper neighbours' interactions are considering
  //H = -J*∑{S_i*S_j} - B*∑{S_i}
  //In this calculation, J = 1, Ferromagnetic, and B = 0, no external magnetic field.
  for(int i(0);i<grid_points;++i)Teng-=spins[i]*(spins[neighbours[i][0]]
                                                +spins[neighbours[i][1]]);
}

//Magnetization density function
void Ising::total_magnetization()
{
  Tmag=0;
  //Calculate total magnetization through whole grid
  //Add absolute value of each spins. It is equal to grid points at the begenning
  for(int i : spins)Tmag+=i;
  Tmag = abs(Tmag);
}

//Specific heat
void Ising::specific_heat(double variance)
{
  //Calculate varience of the rest part
  SH = variance/Temp/Temp/grid_points;
}

//Magnetic susceptibility
void Ising::magnetic_susceptibility(double variance)
{
  //Calculate varience value of the equilibrated part
  MS = variance/Temp/grid_points;
}
