#include"statistics.h"
#include<iostream>
#include<cmath>
 //Call it by reference via &
double mean(std::vector<double>& raw_data, int& length)
{
  //Initialize summation and length of the array
  double sum = 0.0;
  // Adding one by one whole elements of the array
  for(int i = 0;i<length;i++)
  {
    sum += raw_data.at(i);
  }
  // Calculate average of the given sequence
  double result = sum/length;
  return result;
}

double varience(std::vector<double>& raw_data, double& mean, int& length)
{ //Initialize summation
  double sum = 0.0;
  for(int i = 0;i<length;i++)
  { //Hold the difference on a temperory variable to take square
    double temp = raw_data.at(i)-mean;
    sum += temp*temp;
  }
  //Calculate varience; o^2
  return sum/(length-1);
}

std::vector<double> power_fitting(std::vector<double>& x_values, std::vector<double>& y_values, int& length)
{
  //Initialize result vector and summation values
  std::vector<double> params;
  double Sum_x=0,Sum_xx=0,Sum_y=0,Sum_xy=0;
  for(int i=0;i<length;i++)
  {
    Sum_xy += log(x_values[i])*log(y_values[i]);
    Sum_x += log(x_values[i]);
    Sum_y += log(y_values[i]);
    Sum_xx += log(x_values[i])*log(x_values[i]);
  }
  //Calculate fitting parameters b and a
  double param_b = length*Sum_xy-Sum_x*Sum_y;
  param_b /= length*Sum_xx-Sum_x*Sum_x;
  double param_a = Sum_y-param_b*Sum_x;
  param_a /= length;
  param_a = exp(param_a);
  params.push_back(param_a);
  params.push_back(param_b);
  std::cout<<"a value = "<<param_a<<" and b value = "<<param_b<<"\n";
  return params;
}
