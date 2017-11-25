#!/bin/python3
# -*- coding: utf-8 -*-
from runner import Runner
import pandas as pd
from itertools import product
from analyze import Analyzer
import matplotlib.pyplot as plt
import seaborn as sns

sns.set(context="poster", style='white')


class ErrorAnalysis(Runner):
    def setup(self):
        self['dimensions'] = 1
        self['initial condition'] = 'zero'
        self['lower bound'] = 0.0
        self['upper bound'] = 1.0

    def run(self):
        steps = [(1/10, 3e3), (1/100, 3e4)]
        methods = ['forward', 'backward', 'crank-nicolson']
        df = pd.DataFrame(columns=['method', 'dx', 'time', 'initial', 'final', 'y'])
        i = 0
        for (dx, tsteps), method in product(steps, methods):
            self['method'] = method
            self['step size dx'] = dx
            self['number of t points'] = tsteps
            print(f"Running dx={dx}, over {tsteps} time points and method {method}")
            self.run_simulation()
            print("Running error analysis")
            analyzer = Analyzer(self.base_path, 'meta.json')
            time, y, (initial, final) = analyzer.calculate_error([10, -10])
            df.loc[i] = [method, dx, time, initial, final, y]
            i += 1

        self.plot(df)

    def plot(self, df):
        fig, ax = plt.subplots(nrows=3, sharex=True)
        palette = sns.color_palette("GnBu_d", len(df['method']))
        for j, method in enumerate(set(df['method'])):
            dfm = df[df['method'] == method]
            i = 0
            ax[j].set_title(method)
            if j == 1:
                ax[j].set_ylabel("Absolute error")
            for index, row in dfm.iterrows():
                initial = row['initial']
                final = row['final']
                dx = row['dx']
                y = row['y']
                ax[j].plot(y, initial, '-', label=rf'$dx = {dx}$', c=palette[i])
                ax[j].plot(y, final, '--', c=palette[i])
                # ax[j].set_yscale('log')
                i += 1
        fig.legend(loc='upper center', ncol=2)
        fig.savefig('../latex/figures/opg5c.eps')
        plt.show()



if __name__ == '__main__':
    with ErrorAnalysis() as analysis:
        analysis.run()
