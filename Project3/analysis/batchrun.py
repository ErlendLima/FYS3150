#!/bin/python3
# -*- coding: utf-8 -*-

import subprocess
import json
import numpy as np
import pandas as pd
import re
import matplotlib.pyplot as plt
import seaborn as sns
from runner import Runner
sns.set(context='poster')


class BatchRunner(Runner):
    time_pattern = re.compile('CPU time:\s*(.*?)s.*')

    def setup_parameters(self, min):
        self['number of years'] = 1
        self['do save results'] = True
        self['use all planets'] = False
        self['use planets'] = ['Sun', 'Earth']
        self['steps per year'] = min

    def get_relative_total_energy(self):
        """ Calculates the relative total energy at the final step """
        energies = np.loadtxt(self.energy_path)
        start = energies[[1, 2], 0].sum()
        end = energies[[1, 2], -1].sum()
        total_relative = end/start
        return total_relative

    def get_simulation_time(self):
        out = self.run_simulation()
        time = float(out.split()[-1][:-1])  # Ugly ass code. Regex doesn't work
        return time

    def update_json(self, step, method):
        self.increase_timestep(step)
        self.set_method(method)

    def run(self, steps=[10, 20, 30, 50, 100, 200, 500,
                         1000, 1500, 2000, 5000]):
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
                self['method'] = method
                self['steps per year'] += step
                self.data[method + ' time'][counter] = self.get_simulation_time()
                self.data[method][counter] = self.get_relative_total_energy()
            self.data['step'][counter] = step
            counter += 1

        self.data.to_csv('runnerdata.csv')
        self.revert_parameters()

    def load(self, path):
        self.data = pd.DataFrame.from_csv(path)

    def plot(self, load=False):
        if load:
            self.load('runnerdata.csv')

        fig, (error, time) = plt.subplots(nrows=2)
        error.loglog(self.data['step'], self.data['euler'], label='Euler')
        error.loglog(self.data['step'], self.data['verlet'], label='Verlet')
        error.set_xlabel("Stepsize")
        error.set_ylabel("Relative energy after one orbit")
        time.plot(self.data['step'], self.data['euler time'], label='Euler')
        time.plot(self.data['step'], self.data['verlet time'], label='Verlet')
        error.set_xlabel("Stepsize")
        time.set_ylabel("Time [s]")
        fig.legend()
        fig.savefig('../latex/figures/timing.eps')
        plt.show()


if __name__ == '__main__':
    runner = BatchRunner()
    runner.run()
    runner.plot()
