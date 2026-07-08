#include"statistics.hpp"
#include<iostream>
#include<vector>
#include<cmath>
#include<random>
#include<chrono>
 //Call it by reference via &
double mean_value(std::vector<double>& raw_data, double filter)
{
  //calculate length of the given data
  int lattice_length = raw_data.size();
  //Initialize summation and length of the array
  double sum = 0.0;
  // Adding one by one whole elements of the array
  for(std::vector<double>::iterator i=raw_data.begin()+filter*lattice_length;i!=raw_data.end();i++)
  {
    sum += *i;
  }
  // Calculate average of the given sequence
  double result = sum/(lattice_length*(1-filter));
  return result;
}

double variance(std::vector<double>& raw_data, double& mean, double filter)
{ //Calculate length of the given data
  int lattice_length = raw_data.size();
  //Initialize summation
  double sum = 0.0;
  for(std::vector<double>::iterator i=raw_data.begin()+filter*lattice_length;i!=raw_data.end();i++)
  { //Hold the difference on a temporary variable to take square
    double temp = *i-mean;
    sum += temp*temp;
  }
  //Calculate varience; o^2
  return sum/(lattice_length*(1-filter));
}

std::vector<double> power_fitting(std::vector<double>& x_values, std::vector<double>& y_values)
{ //Calculate length of the vectors. They are in same size
  int lattice_length = x_values.size();
  //Initialize result vector and summation values
  std::vector<double> params;
  double Sum_x=0,Sum_xx=0,Sum_y=0,Sum_xy=0;
  for(int i=0;i<lattice_length;i++)
  {
    Sum_xy += log(x_values[i])*log(y_values[i]);
    Sum_x += log(x_values[i]);
    Sum_y += log(y_values[i]);
    Sum_xx += log(x_values[i])*log(x_values[i]);
  }
  //Calculate fitting parameters b and a
  double param_b = lattice_length*Sum_xy-Sum_x*Sum_y;
  param_b /= lattice_length*Sum_xx-Sum_x*Sum_x;
  double param_a = Sum_y-param_b*Sum_x;
  param_a /= lattice_length;
  param_a = exp(param_a);
  params.push_back(param_a);
  params.push_back(param_b);
  std::cout<<"a value = "<<param_a<<" and b value = "<<param_b<<"\n";
  return params;
}
