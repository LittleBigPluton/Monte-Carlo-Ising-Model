#!/usr/bin/env python3

from pathlib import Path
from matplotlib import pyplot
import pandas as pd

script_directory = Path(__file__).resolve().parent
root_directory = script_directory.parent

summary_dir = root_directory / "results" / "temperature_scan" / "summary"
plots_dir = root_directory / "plots" / "temperature_scan"
plots_dir.mkdir(parents=True, exist_ok=True)

#Read scanned data from the observables_scan.dat file
scan_data = pd.read_csv(summary_dir / "observables_scan.dat",sep=r"\s+")
stats_temp_scan = pd.read_csv(summary_dir / "stats_temp_scan.dat",sep=r"\s+")

#To plot average energy density vs temperature graph
y_axises = {"Energy":["AED","SH"],"Magnetization":["AMD","MS"]}
y_labels = {"Energy":["Average Energy Density","Specific Heat"],"Magnetization":["Average Magnetization Density","Magnetic Susceptibility"]}
sub_y_axises = {"Energy":["E_SE","SH_E"],"Magnetization":["M_SE","MS_E"]}
tau_types = {"Energy":"E_Tau","Magnetization":"M_Tau"}
legends = {"Energy":["Standard Error","Error Propagation"],"Magnetization":["Standard Error","Error Propagation"]}
titles = {"Energy":["AED vs Temperature","SH vs Temperature"],"Magnetization":["AMD vs Temperature","MS vs Temperature"]}
observable_types = ["Energy","Magnetization"]

#X axis is shared for temperature scan plots
x_axis = scan_data["Temperature"]

#First loop to decide observable type
for name in observable_types:
    #Second loop to plot desired statistics
    for observable_index in range(0,2):
        #Create three different features' plots on the same figure
        fig, (fig1,fig2,fig3) = pyplot.subplots(3,1,sharex=True,figsize=(11.69,8.27),gridspec_kw={"height_ratios":[3,1,1]})
        fig3.set_xlabel("Temperature")
        subplots = [fig1,fig2,fig3]

        for subplot, feature_index in zip(subplots,range(1,4)):
            #Main line plot
            if feature_index == 1:
                y_axis = y_axises[name][observable_index]
                subplot.plot(x_axis,scan_data[y_axis],marker="o",label="Data")
                subplot.set_ylabel(y_labels[name][observable_index])
                subplot.set_title(titles[name][observable_index])
                subplot.grid(True)
            #Error and autocorrelation plots
            else:
                if feature_index == 2:
                    y_axis = sub_y_axises[name][observable_index]
                    label = legends[name][observable_index]
                    color = "darkred"
                    y_label = "Error"
                else:
                    y_axis = tau_types[name]
                    label = "Autocorrelation Time"
                    color = "black"
                    y_label = "Tau"

                subplot.scatter(x_axis,stats_temp_scan[y_axis],c=color,label=label)
                subplot.set_ylabel(y_label)
                subplot.grid(True)

            subplot.legend()

        pyplot.tight_layout()
        pyplot.savefig(plots_dir / f"{name.lower()}_{y_axises[name][observable_index]}_temperature_scan.png",dpi=300,bbox_inches="tight")
        pyplot.close()
