#!/bin/python3
# -*- coding: utf-8 -*-
from runner import Runner
import pandas as pd
import numpy as np
from itertools import product
from analyze import Analyzer
import matplotlib.pyplot as plt
import seaborn as sns

sns.set(context="poster", style='white')


class ErrorAnalysis(Runner):
    def setup(self):
        self['dimensions'] = 1
        self['initial condition'] = 'sin'
        self['lower bound'] = 0.0
        self['upper bound'] = 1.0

    def run(self):
        steps = [(1/10, 3e3), (1/100, 3e4)]
        methods = ['forward', 'backward', 'crank-nicolson']
        df = pd.DataFrame(columns=['method', 'dx', 'time',
                                   'initial', 'final', 'y',
                                   'total abs', 'total rel'])
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
            total_abs = analyzer.calculate_total_abs_err(method=np.max)
            total_rel = analyzer.calculate_total_rel_err(method=np.max)
            df.loc[i] = [method, dx, time, initial, final, y, total_abs, total_rel]
            i += 1

        self.plot_total_error(df)

    def plot(self, df):
        fig, ax = plt.subplots(nrows=3, sharex=True)
        palette = sns.color_palette("GnBu_d", len(df['method']))
        lines = {}
        for j, method in enumerate(set(df['method'])):
            dfm = df[df['method'] == method]
            i = 0
            ax[j].set_title(method)
            if j == 1:
                ax[j].set_ylabel("Absolute error")
            lines = []
            for index, row in dfm.iterrows():
                initial = row['initial']
                final = row['final']
                dx = row['dx']
                y = row['y']
                l1, = ax[j].plot(y, initial, '-', c=palette[i])
                l2, = ax[j].plot(y, final, '--', c=palette[i])
                i += 1
                lines.append([l1, l2])
        ax[-1].set_xlabel("Position")

        # print([rf"$dx = {dx}$" for dx in set(df['dx'])])
        legend1 = fig.legend([l[0] for l in lines], [rf"$dx = {dx}$" for dx in set(df['dx'])],
                             loc='upper center', ncol=1)
        fig.legend([lines[0][0], lines[0][1]], ['initial', 'final'], loc='upper left', ncol=1)
        # ax = plt.gca().add_artist(legend1)
        fig.savefig('../latex/figures/opg5c.eps')
        plt.show()

    def plot_total_error(self, df):
        fig, ax = plt.subplots(nrows=3, sharex=True)
        for i, method in enumerate(df['method']):
            dfm = df[df['method'] == method]
            for index, row in dfm.iterrows():
                ax[i].plot(dfm['total_rel'])
        plt.show()


if __name__ == '__main__':
    with ErrorAnalysis() as analysis:
        analysis.run()
