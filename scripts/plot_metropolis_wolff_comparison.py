#!/usr/bin/env python3
from pathlib import Path
from matplotlib import pyplot
import pandas as pd

script_directory = Path(__file__).resolve().parent
root_directory = script_directory.parent

summary_dir = root_directory / "results" / "comparison" / "summary"
plots_dir = root_directory / "plots" / "comparison"
plots_dir.mkdir(parents=True, exist_ok=True)

y_axises = {"Energy": ["AED", "SH"], "Magnetization": ["AMD", "MS"]}
y_labels = {"Energy": ["Average Energy Density", "Specific Heat"], "Magnetization": ["Average Magnetization Density", "Magnetic Susceptibility"]}
sub_y_axises = {"Energy": ["E_SE", "SH_E"], "Magnetization": ["M_SE", "MS_E"]}
tau_types = {"Energy": "E_Tau", "Magnetization": "M_Tau"}
legends = {"Energy": ["Standard Error", "Error Propagation"], "Magnetization": ["Standard Error", "Error Propagation"]}
titles = {"Energy": ["AED vs Temperature", "SH vs Temperature"], "Magnetization": ["AMD vs Temperature", "MS vs Temperature"]}
observable_types = ["Energy", "Magnetization"]

##########################################
#####        Phase Region Scan       #####
##########################################
##### Obsevable Plots vs Temperature #####
##########################################

phase_data_mtrpls = pd.read_csv(summary_dir / "phase_scan_mtrpls.dat", sep=r"\s+")
stats_phase_scan_mtrpls = pd.read_csv(summary_dir / "stats_phase_scan_mp.dat", sep=r"\s+")
phase_data_clstr = pd.read_csv(summary_dir / "phase_scan_cluster.dat", sep=r"\s+")
stats_phase_scan_clstr = pd.read_csv(summary_dir / "stats_phase_scan_cl.dat", sep=r"\s+")
x_axis = phase_data_mtrpls["Temperature"]

#First for loop to decide observable type
for name in observable_types:
    #Second for loop to plot desired statistics
    for observable_index in range(0, 2):
        #Third for loop to create three different features' plots on a same figure
        # Primary or secondary observable, error on it and autocorrelation time
        fig, (fig1, fig2, fig3) = pyplot.subplots(3, 1, sharex=True, figsize=(11.69, 8.27), gridspec_kw={"height_ratios": [3, 1, 1]})
        fig3.set_xlabel("Temperature")
        subplots = [fig1, fig2, fig3]

        for subplot, feature_index in zip(subplots, range(1, 4)):
            # Main observable plot
            if feature_index == 1:
                y_axis = y_axises[name][observable_index]
                subplot.plot(x_axis, phase_data_mtrpls[y_axis], marker="o", c="orange", label="Metropolis Data")
                subplot.plot(x_axis, phase_data_clstr[y_axis], marker="o", c="black", label="Wolff Cluster Data")
                subplot.set_ylabel(y_labels[name][observable_index])
                subplot.set_title(titles[name][observable_index])
                subplot.grid(True)

            else:
                # Error plot
                if feature_index == 2:
                    y_axis = sub_y_axises[name][observable_index]
                    label1 = "Metropolis " + legends[name][observable_index]
                    label2 = "Wolff Cluster " + legends[name][observable_index]
                    color1 = "darkred"
                    color2 = "darkgreen"
                    y_label = "Error"

                    subplot.scatter(x_axis, stats_phase_scan_mtrpls[y_axis], c=color1, label=label1)
                    subplot.scatter(x_axis, stats_phase_scan_clstr[y_axis], c=color2, label=label2)

                # Autocorrelation plot
                else:
                    y_axis = tau_types[name]
                    label1 = "Metropolis AC Time"
                    label2 = "Wolff Cluster AC Time in Sweeps"
                    color1 = "black"
                    color2 = "deepskyblue"
                    y_label = "Tau"

                    subplot.scatter(x_axis, stats_phase_scan_mtrpls[y_axis], c=color1, label=label1)
                    subplot.scatter(x_axis, stats_phase_scan_clstr[y_axis] * stats_phase_scan_clstr["CLDav"], c=color2, label=label2)

                subplot.set_ylabel(y_label)
                subplot.grid(True)

            subplot.legend()

        pyplot.tight_layout()
        pyplot.savefig(plots_dir / f"{name.lower()}_{y_axises[name][observable_index]}_comparison.png", dpi=300, bbox_inches="tight")
        pyplot.close()

##########################################
#####        Phase Region Scan       #####
##########################################
#####      Cluster Density Graph     #####
##########################################
figure, (fig_av, fig_var) = pyplot.subplots(2, 1, sharex=True, figsize=(8.27, 7.5), gridspec_kw={"height_ratios": [3, 1]})

fig_av.plot(x_axis, stats_phase_scan_clstr["CLDav"], marker="o", label="Data")
fig_av.set_ylabel("Average Cluster Density")
fig_av.set_title("Average Cluster Density vs Temperature")
fig_av.grid(True)
fig_av.legend()

fig_var.scatter(x_axis, stats_phase_scan_clstr["CLDvar"])
fig_var.set_xlabel("Temperature")
fig_var.set_ylabel("Variance")
fig_var.grid(True)

pyplot.tight_layout()
pyplot.savefig(plots_dir / "cluster_density_comparison.png", dpi=300, bbox_inches="tight")
pyplot.close()
