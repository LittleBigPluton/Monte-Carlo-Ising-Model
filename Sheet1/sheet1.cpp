#include<iostream>
#include<vector>
#include<random>
#include<chrono>
#include<cmath>
#include<fstream>
#include"statistics.h"

// Create a unique seed depends on the current time
unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
// define a mersenne twister generator
std::mt19937 gen;

//Gaussian Distributed value generator for a given mean and deviation into a vector along desired length
void sample_generator(double mean,double deviation,std::mt19937& gen,std::vector<double>& sample_data,int sample_long)
{   //Define the type of distribution function
    std::normal_distribution<double> normdist(mean,deviation);
    //Initialize vector and push back generated numbers inside
    for(int i=0;i<sample_long;i++)sample_data.push_back(normdist(gen));
}

//Logarithmic scale range generator with starting and end points included.
void range_generator(std::vector<double>& range,double min, double max, int samplerange){
  double step_size = (max-min)/(samplerange-1);//Step size between two point
  for(double i=min; i<max; i=i+step_size)range.push_back(round(pow(10,i)));
}

//Cosine calculator for a given range
void cos_estimator(std::vector<double>& results,double mean,double deviation,std::mt19937& gen, int sample_length){
  //Define the type of distribution function
  std::normal_distribution<double> normdist(mean,deviation);
  for(int i=0;i<sample_length;i++)results.push_back(cos(normdist(gen)));
}

int main()
{
//----------------- PART I ------------------ //
  //Initialize generator with the seed
  gen.seed(seed);
  //Declare a vector to store random generated data
  std::vector<double> sample_data;
  sample_generator(5.0,2.0,gen,sample_data,1e6);
  int length = sample_data.size();
  //Calculate average
  double average = mean(sample_data,length);
  std::cout<<average<<std::endl;
  //Calcuate deviation
  double deviation = varience(sample_data,average,length);
  std::cout<<std::sqrt(deviation)<<std::endl;

//----------------- PART II ------------------ //
  //--------------- Task I ------------------- //
  //Logarithmic scale points to estimate expectation value of cosine
  std::vector<double> range;
  //File pointer for average cosine values of M tries for N samples
  std::ofstream av_cos("mean.dat",std::ios::out);
  //File pointer for standart deviation values of M tries for N samples
  std::ofstream sd_cos("deviation.dat",std::ios::out);
  //File pointer for power law parameters
  std::ofstream params("params.dat",std::ios::out);
  // Specified N and M values. N for logarithmic scaled range and M for tries for each N samples.
  int N = 101,M = 100;
  //Call logarithmic scaled range function to calculate scaled points
  range_generator(range,1,6,N);
  //Restore standart deviation values to calculate power fit parameters
  std::vector<double> sigma;
  for(int NN : range){
    //Declare temperory mean vector to hold M mean values for each N values seperatly
    std::vector<double> tmp_mean;
    av_cos<<NN<<" ";
    for(int j = 0;j<M;j++){
    //Hold cosine values to calculate average of N cosine values
    std::vector<double> cos_values;
    //Call cosine estimator to calculate cosine values for a given NN samples
    cos_estimator(cos_values,0.0,1.0,gen,NN);
    //Calculate average cosine values of NN samples
    double result = mean(cos_values,NN);
    //Write this average value on the file
    av_cos<<result<<" ";
    //Hold this average value to calculate standart deviation of M tries
    tmp_mean.push_back(result);
    }
  //Calculate average cosine values of M tries for a given N samples
  double average = mean(tmp_mean,M);
  //Push back standart deviation values
  double temp_sigma = sqrt(varience(tmp_mean,average,M));
  sigma.push_back(temp_sigma);
  //Write standart deviation of these M tries on another file
  sd_cos<<temp_sigma<<std::endl;
  av_cos<<std::endl;
  }
  std::vector<double> fitting_parameters = power_fitting(range,sigma,N);
  for (double x:fitting_parameters) params<<x<<std::endl;
  return 0;
}
