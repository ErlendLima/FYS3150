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
SHOWPLOTS = True


def plotwrap(*args, **kwargs):
    """ Wrapper decorator currying the plotable decorator """
    return plotable(*args, savepath='../latex/figures',
                    show=SHOWPLOTS, **kwargs)


class Analyzer:
    def __init__(self, path, *, loadLU=False, loadSpecial=False,
                 loadGeneral=False):
        self.load(path, loadLU, loadSpecial, loadGeneral)
        self.analytic = self.compute_analytic_solution()
        # self.compute_relative_error()
        self.investigate_error()
        # self.make_relative_error_plot()

    def load(self, path, loadLU=False, loadSpecial=False, loadGeneral=False):
        matches = glob(os.path.join(path, '[G|L|S]*.txt'))
        matches.sort()
        self.data = {}
        tag = 'L' if loadLU else (
            'G' if loadGeneral else 'S')

        for match in matches:
            n = re.search('{}(\d+)\.txt'.format(tag), match)
            if n is not None:
                self.data[int(n.group(1))] = np.loadtxt(match)
        self.data = sorted(self.data.items(), key=operator.itemgetter(0))

    def compute_analytic_solution(self, n=1000):
        x = np.linspace(0, 1, n)
        y = 1 - (1-np.exp(-10))*x - np.exp(-10*x)
        #y = np.exp(1)*x-x-np.exp(x)+1
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
            x = np.linspace(0, 1, n)
            ax.plot(x, data, label=make_label(n))
        ax.plot(*self.analytic, label=r'Analytic')
        ax.legend()

    def compute_relative_error(self):
        rows = []
        for n, data in self.data:
            # Ignoring boundary terms since they are correct
            analytic = self.compute_analytic_solution(n)[1][1:-1]
            err = np.log10(np.abs((data[1:-1] - analytic)/analytic))
            maxerr = np.max(err)
            rows.append([n, maxerr])

        table = tag('error_table', tabulate(rows, headers=["n", "log error"],
                                            tablefmt="latex"))
        print(table)

    def investigate_error(self):
        for n, data in self.data:
            x, y = self.compute_analytic_solution(n)
            x, y = x[1:-1], y[1:-1]
            err = np.abs((data[1:-1] - y)/y)
            plt.plot(x, err)
            ind = np.argmax(err)
            plt.plot(x[ind], y[ind], 'o')


    @plotwrap(saveas='error.eps')
    def make_relative_error_plot(self):
        arr = np.loadtxt("../cpp/data/E.txt")
        ar = arr[1:, :]
        ar = ar[ar[:,1].argsort()]
        plt.loglog(ar[1:,0], ar[1:,1])
        plt.scatter(ar[1:,0], ar[1:,1])


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Analyzes data for project 1')
    parser.add_argument('search_path', type=os.path.abspath,
                        help="Directory to search for input files")
    parser.add_argument('-L', '--LU', action='store_true',
                        help="Analyze LU")
    parser.add_argument('-S', '--SP', action='store_true',
                        help="Analyze special")
    parser.add_argument('-G', '--GR', action='store_true',
                        help="Analyze general")
    args = parser.parse_args()
    analyzer = Analyzer(args.search_path, loadLU=args.LU, loadSpecial=args.SP,
                        loadGeneral=args.GR)
    analyzer.plot()
