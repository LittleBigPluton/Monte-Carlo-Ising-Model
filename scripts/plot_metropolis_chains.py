#!/usr/bin/env python3.13

from pathlib import Path
from matplotlib import pyplot
from seaborn import lineplot, scatterplot, set_theme
from pandas import read_csv

# Set figure size as A4 paper
set_theme(rc={'figure.figsize': (8.27, 11.69)})

# Implement dictionary to label each graph uniquely
figure_name = {0: "Average Energy Densities vs ", 1: "Average Magnetization Densities vs ",
               "time": "Monte Carlo Time"}

# Implement dictionary to open different files
file_names = {0: "energies_", 1: "magnetizations_"}

# Implement dictionary to define Y-axis properly
ylabels = {0: "Average Energy Densities \n", 1: "Average Magnetization \n Densities ",
           "chains": ["1st", "2nd", "3rd"]}

y_columns = {0: "Energies", 1: "Magnetizations"}

# Define directories
script_directory = Path(__file__).resolve().parent
root_directory = script_directory.parent
chains_dir = root_directory / "results" / "chains"
summary_dir = root_directory / "results" / "summary"
plots_dir = root_directory / "plots"
plots_chains_dir = plots_dir / "chains"
plots_chains_dir.mkdir(parents=True, exist_ok=True)

# Read observables data from the file
file_path = summary_dir / "observables_metropolis_chains.dat"
observables = read_csv(file_path, sep=r"\s+", index_col=False)

observables_name = {0: "AED", 1: "AMD", 2: "SH", 3: "MS"}
observables_title = {0: "Average Energy Density Through Chains",
                     1: "Average Magnetization Density Through Chains",
                     2: "Specific Heats",
                     3: "Magnetic Susceptibilities"}

# For loop to access different temperature values between 2.0-2.6
# It starts from 20 and ends 26 with step size 3
for temperature in range(20, 29, 3):
    # Changing between data types either energy values or magnetization values
    for observable_type in range(0, 2):
        # Define figure scheme with 3 subplots vertically with same X-axis label
        figure, axes = pyplot.subplots(3, 1, sharex=True)

        # Define title name
        title = figure_name[observable_type] + figure_name["time"] + " at " + str(temperature / 10) + "K"
        figure.suptitle(title)

        # Hiding X-labels between subplots
        figure.subplots_adjust(hspace=0)

        # Open 3 Markov chains for specified data type
        for i in range(0, 3):
            # Define file name
            filename = chains_dir / f"{file_names[observable_type]}{i + 1}_{temperature}.dat"

            # Read data from file
            data = read_csv(filename, sep=r"\s+")

            # Plot lines between data points
            lineplot(data=data, x="Times", y=y_columns[observable_type], ax=axes[i]).set(
                xlabel="Monte Carlo Times",
                ylabel=ylabels[observable_type] + ylabels["chains"][i] + " chain"
            )

        # Save figure as PDF
        pyplot.savefig(plots_chains_dir / f"{file_names[observable_type]}{temperature}.pdf",
                       format="pdf", bbox_inches="tight")
        pyplot.close()

# Plot observables
set_theme(rc={'figure.figsize': (6.4, 4.8)})

for average_type in range(4):
    scatterplot(data=observables, x="Temperature", y=observables_name[average_type], hue="MCS").set(
        title=observables_title[average_type] + " vs Temperature"
    )

    pyplot.savefig(plots_chains_dir / f"{observables_name[average_type]}.png", bbox_inches="tight")
    pyplot.close()
