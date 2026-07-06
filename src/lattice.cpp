#include<iostream>
#include<vector>
#include<random>
#include"lattice.hpp"
//Take input from user for length of the lattice to initialize grid points
void Lattice::initialize(){
  std::cout<<"Enter lattice size: ";
  std::cin>>length;
  grid_points = length*length;
  std::cout<<"2D spin-grid was initialized."<<"\n";
  //Find nearest neighbours' positions of each spin
  nearest_neighbour();

}

void Lattice::nearest_neighbour(){
  //To be sure neighbours vector is empty
  neighbours.clear();
  for(int i=0;i<grid_points;i++){
    //Define temperory coordiantes to locate given point
    int x_temp = i%length;
    int y_temp = i/length;
    //Find nearest neighbour of the given point
    std::vector<int> temp = {
      y_temp*length+((x_temp-1)%length+length)%length,//Left
      (((y_temp-1)%length+length)%length)*length+x_temp,//Upper
      y_temp*length+(x_temp+1)%length,//Right
      ((y_temp+1)%length)*length+x_temp//Down
    };
    neighbours.push_back(temp);
  }
  std::cout<<"NNs were found."<<"\n";
}
