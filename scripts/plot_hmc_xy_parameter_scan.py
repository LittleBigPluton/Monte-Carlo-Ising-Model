#!/usr/bin/env python3

from pathlib import Path

import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns


script_directory = Path(__file__).resolve().parent
root_directory = script_directory.parent

summary_dir = root_directory / "results" / "hmc_xy" / "summary"
plots_dir = root_directory / "plots" / "hmc_xy" / "parameter_scan"
plots_dir.mkdir(parents=True, exist_ok=True)

data_file = summary_dir / "hmc_xy_parameter_scan.csv"

data = pd.read_csv(data_file)
data = data.sort_values(["LeapfrogSteps", "LeapfrogStepSize"])

plots = {
    "AcceptanceRate": "Acceptance Rate",
    "EnergyTau": "Energy Autocorrelation Time",
    "MagnetizationTau": "Magnetization Autocorrelation Time",
    "IndependentTimeEnergy": r"$\tau_{ind}$ Energy",
    "IndependentTimeMagnetization": r"$\tau_{ind}$ Magnetization",
    "MagnetizationDensity": "Magnetization Density",
}

for y_column, y_label in plots.items():
    fig, ax = plt.subplots(figsize=(8, 5))

    sns.lineplot(data=data, x="LeapfrogStepSize", y=y_column, hue="LeapfrogSteps", marker="o", ax=ax)
    ax.set_xlabel(r"Leapfrog Step Size $\Delta t$")
    ax.set_ylabel(y_label)
    ax.set_title(f"{y_label} vs Leapfrog Step Size")
    ax.grid(True)
    ax.legend(title="Leapfrog Steps")

    plt.tight_layout()
    figure_name = f"hmc_xy_parameter_scan_{y_column}.png"
    plt.savefig(plots_dir / figure_name, dpi=300, bbox_inches="tight")
    plt.close()


print(f"Parameter-scan plots saved to: {plots_dir}")
