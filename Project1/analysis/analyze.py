#!/bin/python3
# -*- coding: utf-8 -*-

import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
import argparse
import os
from glob import glob
import re
from tabulate import tabulate
import operator
from latextools import plotable, tag, untag, untag_all

sns.set()
SHOWPLOTS = False


def plotwrap(*args, **kwargs):
    """ Wrapper decorator currying the plotable decorator """
    return plotable(*args, savepath='../latex/figures',
                    show=SHOWPLOTS, **kwargs)


class Analyzer:
    def __init__(self, path):
        self.load(path)
        self.analytic = self.compute_analytic_solution()
        self.compute_relative_error()

    def load(self, path):
        matches = glob(os.path.join(path, 'n*.txt'))
        matches.sort()
        self.data = {}
        for match in matches:
            n = re.search('n(\d+)\.txt', match)
            self.data[int(n.group(1))] = np.loadtxt(match)
        self.data = sorted(self.data.items(), key=operator.itemgetter(0))

    def compute_analytic_solution(self, n=1000):
        x = np.linspace(0, 1, n)
        y = 1 - (1-np.exp(-10))*x - np.exp(-10*x)
        return (x, y)

    @plotwrap(saveas='function.eps')
    def plot(self):
        def make_label(n):
            if n == 10:
                return '$n = 10$'
            else:
                return '$n = 10^{%g}$' % (np.log10(n))

        fig, ax = plt.subplots()
        for n, data in self.data:
            x = np.linspace(0, 1, n+2)
            ax.plot(x, data, label=make_label(n))
        ax.plot(*self.analytic, label=r'Analytic')
        ax.legend()

    def compute_relative_error(self):
        rows = []
        for n, data in self.data:
            # Ignoring boundary terms since they are correct
            analytic = self.compute_analytic_solution(n+2)[1][1:-1]
            err = np.log10(np.abs((data[1:-1] - analytic)/analytic))
            maxerr = np.max(err)
            rows.append([n, maxerr])

        table = tag('error_table', tabulate(rows, headers=["n", "log error"],
                                            tablefmt="latex"))
        print(table)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Analyzes data for project 1')
    parser.add_argument('search_path', type=os.path.abspath,
                        help="Directory to search for input files")
    args = parser.parse_args()
    analyzer = Analyzer(args.search_path)
    analyzer.plot()
