#!/usr/bin/env python3

from pathlib import Path

import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns


script_directory = Path(__file__).resolve().parent
root_directory = script_directory.parent

summary_dir = root_directory / "results" / "hmc_xy" / "summary"
plots_dir = root_directory / "plots" / "hmc_xy" / "temperature_scan"
plots_dir.mkdir(parents=True, exist_ok=True)

data_file = summary_dir / "hmc_xy_temperature_scan.csv"

data = pd.read_csv(data_file)
data = data.sort_values("Temperature")
# Prepare legend labels
data["HMCParameters"] = (r"$\Delta t$ = " + data["LeapfrogStepSize"].astype(str) + ", LF steps = " + data["LeapfrogSteps"].astype(str))

# Calculate and store independent times for energy and magnetization
data["IndependentTimeEnergy"] = 2.0 * data["EnergyTau"] * data["LeapfrogSteps"]
data["IndependentTimeMagnetization"] = 2.0 * data["MagnetizationTau"] * data["LeapfrogSteps"]

fig, ax = plt.subplots(figsize=(8, 5))
for parameter_label, subset in data.groupby("HMCParameters"):
    ax.errorbar(subset["Temperature"], subset["MagnetizationDensity"], yerr = subset["MagnetizationSE"],
                fmt = "o-", capsize = 5, label = parameter_label)

ax.set_xlabel("Temperature")
ax.set_ylabel("Magnetization Density")
ax.set_title("HMC XY Magnetization Density vs Temperature")
ax.grid(True)
ax.legend(title="HMC Parameters")

plt.tight_layout()
plt.savefig(plots_dir / "hmc_xy_magnetization_density_temperature_scan.png", dpi=300,bbox_inches="tight")
plt.close()


fig, ax = plt.subplots(2, 1, figsize=(8, 6), sharex=True)

sns.lineplot(data = data, x = "Temperature", y = "IndependentTimeEnergy",
             hue = "HMCParameters", marker = "o", ax = ax[0])

ax[0].set_title(r"$\tau_{ind}$ vs Temperature")
ax[0].set_ylabel(r"$\tau_{ind}$")
ax[0].legend(title = "HMC Parameters")
ax[0].grid(True)

sns.lineplot(data=data, x="Temperature", y="AcceptanceRate",
             hue="HMCParameters", marker="o", ax=ax[1])

ax[1].set_title("Acceptance Rate vs Temperature")
ax[1].set_xlabel("Temperature")
ax[1].set_ylabel("Acceptance Rate")
ax[1].legend(title = "HMC Parameters")
ax[1].grid(True)

plt.tight_layout()
plt.savefig(plots_dir / "hmc_xy_temperature_scan_diagnostics.png", dpi=300, bbox_inches="tight")
plt.close()


temperature_plots = {
    "EnergyDensity": "Energy Density",
    "EnergyTau": "Energy Autocorrelation Time",
    "MagnetizationTau": "Magnetization Autocorrelation Time",
    "IndependentTimeMagnetization": r"$\tau_{ind}$ Magnetization",
}

for y_column, y_label in temperature_plots.items():
    fig, ax = plt.subplots(figsize=(8, 5))

    sns.lineplot(data=data, x="Temperature", y=y_column, marker="o", hue="HMCParameters", ax=ax)

    ax.set_xlabel("Temperature")
    ax.set_ylabel(y_label)
    ax.set_title(f"{y_label} vs Temperature")
    ax.legend(title = "HMC Parameters")
    ax.grid(True)

    plt.tight_layout()
    figure_name = f"hmc_xy_temperature_scan_{y_column}.png"
    plt.savefig(plots_dir / figure_name, dpi=300, bbox_inches="tight")
    plt.close()


print(f"Temperature-scan plots saved to: {plots_dir}")
