#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <chrono>
#include <fstream>

#include "ising_model.hpp"
#include "metropolis.hpp"
#include "measurements.hpp"
#include "statistics.hpp"

class MonteCarloSimulation
{
private:
    Ising model;
    Metropolis metropolis;
    Measurements measurements;
    MeasurementSummary summary;

    std::mt19937 generator;
    int Nmc = 0;

public:
    void simulation()
    {
        // Create a file to hold primary and secondary data values
        std::ofstream observables("observables.dat", std::ios::out);
        observables << "Temperature MCS AED AMD SH MS" << std::endl;

        // Initialize lattice and grid points
        model.initialize();
        std::cout << "Enter length of Monte Carlo Chains: ";

        // Enter length of the Markov chains
        std::cin >> Nmc;

        // Define temperature values
        double Temperature[3] = {2.0,2.3,2.6};

        for (double Temp : Temperature)
        {
            // Create lookup table for the given temperature
            metropolis.lookup(Temp);

            // Create three unique Monte Carlo chains
            for (int mcgen = 0; mcgen < 3; ++mcgen)
            {
                // Clear measurements before every new chain
                measurements.energies.clear();
                measurements.magnetizations.clear();

                // Create a unique seed
                unsigned seed = static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()) + mcgen;
                generator.seed(seed);

                // Generate spins and calculate initial values
                model.discrete_spin_generator(generator, false, "cold");
                model.calculate_total_energy();
                model.calculate_total_magnetization();
                std::cout << "Temperature = " << Temp << std::endl;

                // Start measuring time
                auto start = std::chrono::high_resolution_clock::now();

                // Run the Metropolis algorithm
                for (int i = 0; i < Nmc; ++i)
                {
                    metropolis.sweep(model, generator, measurements);
                }

                // Ignore the first 10% of the chain
                double filter = 0.1;

                // Calculate mean values
                double average_energy = mean_value(measurements.energies, filter);
                double average_magnetization = mean_value(measurements.magnetizations, filter);

                // Calculate variances
                double energy_variance = variance(measurements.energies, average_energy, filter);
                double magnetization_variance = variance(measurements.magnetizations, average_magnetization, filter );

                // Calculate densities
                summary.energy_density = average_energy / model.grid_points;
                summary.magnetization_density = average_magnetization / model.grid_points;

                // Calculate secondary observables
                calculate_specific_heat(energy_variance,Temp,model.grid_points,summary);
                calculate_magnetic_susceptibility(magnetization_variance,Temp,model.grid_points,summary);

                std::cout << "Specific Heat = " << summary.specific_heat << std::endl
                          << "Magnetic Susceptibility = " << summary.magnetic_susceptibility << std::endl;

                observables << Temp << " " << mcgen << " " << summary.energy_density << " " << summary.magnetization_density << " "
                            << summary.specific_heat << " " << summary.magnetic_susceptibility << std::endl;

                // Stop measuring time
                auto stop = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
                std::cout << "Time taken by function: " << duration.count() << " microseconds" << std::endl;
            }
        }
    }
};

int main()
{
    MonteCarloSimulation object;
    object.simulation();

    return 0;
}
