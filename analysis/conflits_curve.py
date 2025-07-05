###################################################
# Esse arquivo foi criado com a ajuda do Chat GPT #
###################################################

import os
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

def logistic_func(x, L ,k, x0, c):
    return L / (1 + np.exp(-k*(x - x0))) + c

def load_and_aggregate(file_path):
    df = pd.read_csv(file_path)
    df = df.drop(columns=['file_path'])
    means = df.mean()
    return means.values

tabu_means = load_and_aggregate(os.path.join("..", "out" , "ts.csv"))
aco_means = load_and_aggregate(os.path.join("..", "out" , "aco.csv"))

x = np.array([0, 1, 2, 3])

def fit_and_plot(x, y, label, color):
    popt, _ = curve_fit(logistic_func, x, y, p0=[1, 1, 1, 0], maxfev=10000)
    x_fit = np.linspace(0, 6, 300)
    y_fit = logistic_func(x_fit, *popt)
    
    plt.plot(x, y, 'o', color=color, label=f'{label} (mean)')
    plt.plot(x_fit, y_fit, '-', color=color, label=f'{label} (fit)')
    return popt

plt.figure(figsize=(10, 6))
tabu_params = fit_and_plot(x, tabu_means, 'Tabu Search', 'blue')
aco_params = fit_and_plot(x, aco_means, 'ACO', 'green')

plt.xlabel('Δ = k - cores_usadas')
plt.ylabel('Taxa de conflito')
plt.title('Taxa de conflito esperado vs Δ (Extrapolado)')
plt.legend()
plt.grid(True)
plt.show()
