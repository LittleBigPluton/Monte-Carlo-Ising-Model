#include<iostream>
#include<random>
#include<cmath>
#include "wolff_cluster.hpp"

void WolffCluster::sweep(Ising& model,std::vector<double> &cluster_sizes, double& Temperature, std::mt19937& generator){
  //Define random site and uniform random number generators
  std::uniform_int_distribution<int> random_site(0,model.grid_points-1);
  std::uniform_real_distribution<double> uniform_random(0.0,1.0);

  //std::cout<<"Cluster algorithm started"<<std::endl;
  //Create cluster vector to store cluster sites
  std::vector<int> cluster_sites;
  cluster_sites.reserve(model.grid_points);

  //Create seed of the cluster randomly and flip it immediately
  int cluster_seed = random_site(generator);

  //Store the cluster seed
  cluster_sites.push_back(cluster_seed);

  //Change the seed's spin immediately
  model.spins[cluster_seed] *= -1;
  //std::cout<<"spin was flipped"<<std::endl;

  //Define probability of acceptance
  double probability = 1.0-exp(-2.0/Temperature);
  //std::cout<<"probability was calculated"<<std::endl;

  //Start to visit sides of the cluster
  for(std::size_t cluster_index=0;cluster_index<cluster_sites.size();cluster_index++)
  {
    //std::cout<<"Evaluate cluster"<<std::endl;
    //Store Nearest Neighbours of the choosen cluster element
    const std::vector<int> cluster_neighbours = model.neighbours[cluster_sites[cluster_index]];

    for(int neighbour:cluster_neighbours)
    {
      //std::cout<<"Evaluate neighbours"<<std::endl;
      //If initial spin value of the cluster seed is parallel to evaluated neighbour, apply A-R test
      if(-model.spins[cluster_sites[cluster_index]]==model.spins[neighbour])
      {
        //Apply Accept-Reject test with calculated probability
        if(uniform_random(generator)<=probability)
        {
          //Add new side into the cluster sites
          cluster_sites.push_back(neighbour);
          //Flip the new side's spin value immediately
          model.spins[neighbour] *= -1;
          //std::cout<<"new neighbour was added"<<std::endl;
        }//A-R test loop
      }//If loop for parallel spin sides
    }//Neighbour loop
  }//Cluster loop

  //Calculate cluster size density and store
  cluster_sizes.push_back(cluster_sites.size()/(double)model.grid_points);

}//Function end
