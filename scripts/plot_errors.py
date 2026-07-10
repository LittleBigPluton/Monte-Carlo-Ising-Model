#!/usr/bin/env python3

# Import only read_csv function from pandas to read data from file
from pandas import read_csv
# Import lineplot from seaborn library to make plots
from seaborn import lineplot
# Import numpy functions
from numpy import exp, array, isfinite
# Import pyplot functions from matplotlib library
from matplotlib import pyplot
# Import curve_fit function from Scipy library to apply exponential fit on the graphs
from scipy.optimize import curve_fit
# Import directory management library
from pathlib import Path

# Detect main directory
script_directory = Path(__file__).resolve().parent
root_directory = script_directory.parent

# Define input data directories
summary_dir = root_directory / "results" / "summary"
decay_data_dir = root_directory / "results" / "autocorrelation"

# Define export data directories
plots_dir = root_directory / "plots"
error_plots_dir = plots_dir / "errors"
decay_plots_dir = plots_dir / "autocorrelation"

# Check export directories exist or create
error_plots_dir.mkdir(parents=True, exist_ok=True)
decay_plots_dir.mkdir(parents=True, exist_ok=True)

# Define headers and file suffixes
observables = {1: ["AED", "SH"], 2: ["AMD", "MS"]}
errors = {1: "EP", 2: "BM", 3: "BSM"}
error_types = {1: "Error Propagation", 2: "Blocking Method", 3: "Bootstrap Method"}
secondary_observables = {1: "Specific Heat", 2: "Magnetic Susceptibility"}

# Import observable data
observables_errors_file = summary_dir / "observables_error_analysis.dat"
observable_data = read_csv(observables_errors_file, sep=r"\s+", index_col=False)

# Import error data
error_data_file = summary_dir / "errors.dat"
error_data = read_csv(error_data_file, sep=r"\s+", index_col=False)

# Extract error on specific heat and magnetic susceptibility
SH_error = error_data.loc[error_data["type"] == "E"].reset_index(drop=True)
MS_error = error_data.loc[error_data["type"] == "M"].reset_index(drop=True)

# Import simulation temperatures
Temperatures = array(observable_data["Temperature"])

for sec_obs in range(1, 3):
    if sec_obs == 1:
        observable = array(observable_data["SH"])
        selected_error = SH_error
    else:
        observable = array(observable_data["MS"])
        selected_error = MS_error

    for error_type in range(1, 4):
        error_column = errors[error_type]
        pyplot.figure()
        pyplot.grid(visible=True, which="major", axis="both", linestyle="-", linewidth=0.5)
        pyplot.errorbar(x=Temperatures, y=observable, yerr=selected_error[error_column], fmt="o", capsize=4)
        pyplot.title("Error on {secondary_observable} with {error_type}".format(secondary_observable=secondary_observables[sec_obs],
                     error_type=error_types[error_type]))
        pyplot.ylabel(secondary_observables[sec_obs])
        pyplot.xlabel("Temperature")
        pyplot.savefig(error_plots_dir / f"{secondary_observables[sec_obs].replace(' ', '_')}_{error_column}.png", bbox_inches="tight")
        pyplot.close()

# Define exponential function to apply exponential fitting
def func(x, tau):
    return exp(-x / tau)

# Define figure and file names for automation
figure_names_decay = {1: "Energy Decay Rate at ", 2: "Magnetization Decay Rate at "}
file_names = {1: "Energy_decay_", 2: "Magnetization_decay_"}

for temp in range(20, 29, 3):
    for observable_type in range(1, 3):
        # Generate file name with various temperatures
        file_name = decay_data_dir / f"{file_names[observable_type]}{temp}.dat"

        # Read data from file
        decay_values = read_csv(file_name, sep=r"\s+")

        # The time starts from the exported data file
        times = array(decay_values["Times"])

        # Normalise decay values with first value
        decay_raw = array(decay_values["Decay"])

        if len(decay_raw) == 0 or decay_raw[0] == 0:
            print("Skipping invalid decay file:", file_name)
            continue

        decay = decay_raw / decay_raw[0]

        # Remove invalid values before fitting
        valid = isfinite(times) & isfinite(decay)
        times = times[valid]
        decay = decay[valid]

        if len(times) < 2:
            print("Not enough data points for fit:", file_name)
            continue

        # Create a figure environment to plot decay values
        pyplot.figure(figure_names_decay[observable_type] + str(temp))

        # Plot the values
        lineplot(x=times, y=decay, marker="o", linestyle="").set(xlabel="Auto Correlation Time (t)",
                 ylabel=r"$\rho(t)$", title=figure_names_decay[observable_type] + str(temp / 10))

        # Calculate fitting parameters using curve_fit function
        try:
            popt, pcov = curve_fit(func, times, decay)
        except RuntimeError:
            print("Fit failed for", file_name)
            pyplot.close()
            continue

        print(file_name.name, "tau fit =", popt[0])

        # Plot exponential fit
        fitted_y = func(times, *popt)
        lineplot(x=times, y=fitted_y)

        pyplot.grid(visible=True, which="major", linewidth=0.5)
        pyplot.grid(visible=True, which="minor", linewidth=0.35)
        pyplot.minorticks_on()
        pyplot.savefig(decay_plots_dir / f"{file_names[observable_type]}{temp}.png", bbox_inches="tight")
        pyplot.close()
