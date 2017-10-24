#!/bin/python3
# -*- coding: utf-8 -*-

import subprocess
import json
import numpy as np
import pandas as pd
import re
import matplotlib.pyplot as plt
import seaborn as sns
sns.set(context='poster')


class Runner:
    time_pattern = re.compile('CPU time:\s*(.*?)s.*')

    def __init__(self, exe_path, param_path, energy_path):
        self.exe_path = exe_path
        self.param_path = param_path
        self.energy_path = energy_path

    def setup_parameters(self, min):
        with open(self.param_path, 'r') as param_file:
            param_json = json.load(param_file)
        param_json['number of years'] = 1
        param_json['do save results'] = True
        param_json['use all planets'] = False
        param_json['use planets'] = ['Sun', 'Earth']
        param_json['steps per year'] = min
        with open(self.param_path, 'w') as param_file:
            json.dump(param_json, param_file, indent=4)

    def increase_timestep(self, step=100):
        """ Increase the parameter 'steps per year' by step """
        self.param_json["steps per year"] = int(self.param_json["steps per year"]) + step

    def set_method(self, method):
        """ Sets the integration method to either Euler or Verlet """
        self.param_json["method"] = method

    def get_relative_total_energy(self):
        """ Calculates the relative total energy at the final step """
        energies = np.loadtxt(self.energy_path)
        start = energies[[1, 2], 0].sum()
        end = energies[[1, 2], -1].sum()
        total_relative = end/start
        return total_relative

    def run_simulation(self):
        process = subprocess.Popen(self.exe_path, stdout=subprocess.PIPE,
                                   stderr=subprocess.PIPE, shell=True)
        out, _ = process.communicate()
        out = out.decode('ascii')
        time = float(out.split()[-1][:-1])  # Ugly ass code. Regex doesn't work
        return time

    def update_json(self, step, method):
        """ Update the parameters of the json file """
        with open(self.param_path, 'r') as param_file:
            self.param_json = json.load(param_file)
        self.increase_timestep(step)
        self.set_method(method)
        with open(self.param_path, 'w') as param_file:
            json.dump(self.param_json, param_file, indent=4)

    def run(self, steps=[10, 20, 30, 50, 100, 200, 500,
                         1000, 1500, 2000, 5000, 10_000, 100_000]):
        self.setup_parameters(min=min(steps))
        num_steps = len(steps)
        self.data = pd.DataFrame({'verlet': np.zeros(num_steps),
                                  'euler': np.zeros(num_steps),
                                  'verlet time': np.zeros(num_steps),
                                  'euler time': np.zeros(num_steps),
                                  'step': np.zeros(num_steps)})
        counter = 0
        for step in steps:
            step = int(step)
            print(f"{counter}: {step}")
            for method in ['verlet', 'euler']:
                self.update_json(step, method)
                self.data[method + ' time'][counter] = self.run_simulation()
                self.data[method][counter] = self.get_relative_total_energy()
            self.data['step'][counter] = step
            counter += 1

        self.data.to_csv('runnerdata.csv')
        self.plot()

    def load(self, path):
        self.data = pd.DataFrame.from_csv(path)

    def plot(self, load=False):
        if load:
            self.load('runnerdata.csv')

        fig, (error, time) = plt.subplots(nrows=2)
        error.loglog(self.data['step'], self.data['euler'], label='Euler')
        error.loglog(self.data['step'], self.data['verlet'], label='Verlet')
        error.set_xlabel("Stepsize")
        error.set_ylabel("Relative error after 1 orbit")
        time.plot(self.data['step'], self.data['euler time'], label='Euler')
        time.plot(self.data['step'], self.data['verlet time'], label='Verlet')
        error.set_xlabel("Stepsize")
        time.set_ylabel("Time [s]")
        fig.legend()
        plt.show()


if __name__ == '__main__':
    runner = Runner('../cpp/solve', '../data/parameters.json', '../data/energy.txt')
    runner.run()
