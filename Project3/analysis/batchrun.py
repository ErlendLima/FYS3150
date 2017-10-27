#!/bin/python3
# -*- coding: utf-8 -*-

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
        self['freeze sun'] = True
        self['use two body approximation'] = False

    def get_relative_total_energy(self):
        """ Calculates the relative total energy at the final step """
        energies = self.get_energy()
        start = energies[[1, 2], 0].sum()
        end = energies[[1, 2], -1].sum()
        total_relative = end/start
        return total_relative

    def run(self, steps=[10, 20, 30, 50, 100, 200, 500,
                         1000, 1500, 2000, 3000, 5000,
                         8000, 10000, 50000, 100000, 1000000]):
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
            self['steps per year'] += step
            print("{}: {}".format(counter, step))
            for method in ['verlet', 'euler']:
                self['method'] = method
                out, _ = self.run_simulation()
                self.data[method + ' time'][counter] = self.extract_time(out)
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

        fig, (error, time) = plt.subplots(nrows=2, figsize = (10,10), sharex=True)
        error.semilogx(self.data['step'], self.data['euler'], label='Euler')
        error.semilogx(self.data['step'], self.data['verlet'], label='Verlet')
        # error.set_xlabel("Steps per year")
        error.set_ylabel("Energy")
        time.semilogx(self.data['step'], self.data['euler time'], label='Euler')
        time.semilogx(self.data['step'], self.data['verlet time'], label='Verlet')
        time.set_ylabel("Time [s]")
        time.set_xlabel("Number of time steps")
        fig.legend(loc="center", ncol=2)
        fig.savefig('../latex/figures/timing.eps')
        plt.show()


if __name__ == '__main__':
    runner = BatchRunner()
    runner.run()
    runner.plot()
