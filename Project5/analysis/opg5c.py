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
        self['upper bound'] = 0.0

    def run(self):
        steps = [(1/10, 3e3), (1/100, 3e4)]
        methods = ['forward', 'backward', 'crank-nicolson']
        df = pd.DataFrame(columns=['method', 'dx',
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
            # analyzer.plot()
            y, (initial, final) = analyzer.calculate_rel_err([0.1, 0.8])
            total_abs = analyzer.calculate_total_abs_err(method=np.max, tlimits=[1, 1000])
            total_rel = analyzer.calculate_total_rel_err(method=np.max, max_time=1.2)
            df.loc[i] = [method, dx, initial, final,
                         y, total_abs, total_rel]
            i += 1

        self.plot_total_error(df)
        self.plot_pos_error(df)

    def plot_pos_error(self, df):
        fig, ax = plt.subplots(nrows=2, sharex=True, figsize=(9.5, 8))
        pal = sns.color_palette()
        for j, dx in enumerate(set(df['dx'])):
            dfm = df[df['dx'] == dx]
            i = 0
            ax[j].set_title(rf'dx = {dx}')
            for index, row in dfm.iterrows():
                initial = row['initial']
                final = row['final']
                method = row['method']
                y = row['y']
                l1, = ax[j].plot(y, initial, '-', label=rf'{method}', c=pal[i])
                l2, = ax[j].plot(y, final, '--', c=pal[i])
                i += 1

        # Shrink current axis by 20%
        for a in ax:
            box = a.get_position()
            a.set_position([box.x0, box.y0, box.width * 0.8, box.height])

        # Put a legend to the right of the current axis
        fig.legend(loc='center right')
        ax[-1].set_xlabel("Position")
        ax[0].set_ylabel("Relative error")
        ax[1].set_ylabel("Relative error")

        fig.savefig('../latex/figures/relative_error_in_position.png')

    def plot_total_error(self, df):
        fig, ax = plt.subplots(nrows=2, sharex=True, figsize=(9.5, 8))
        for i, dx in enumerate(set(df['dx'])):
            dfm = df[df['dx'] == dx]
            ax[i].set_title(rf"dx = {dx}")
            for index, row in dfm.iterrows():
                time, err = row['total rel']
                method = row['method']
                ax[i].plot(time, err, label=rf"{method}")
        # Shrink current axis by 20%
        for a in ax:
            box = a.get_position()
            a.set_position([box.x0, box.y0, box.width * 0.8, box.height])

        # Put a legend to the right of the current axis
        fig.legend(loc='center right')
        ax[-1].set_xlabel('Time [s]')
        ax[1].set_ylabel('Max Relative Error')
        ax[0].set_ylabel('Max Relative Error')
        fig.savefig('../latex/figures/relative_error_in_time.png')


if __name__ == '__main__':
    with ErrorAnalysis() as analysis:
        analysis.run()
