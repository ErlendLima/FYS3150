#!/bin/python3
# -*- coding: utf-8 -*-

import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
import argparse
import os
from glob import glob
import re

sns.set()


class Analyzer:
    def __init__(self, path):
        self.load(path)
        self.compute_analytic_solution()

    def load(self, path):
        matches = glob(os.path.join(path, 'n*.txt'))
        matches.sort()
        self.data = {}
        for match in matches:
            n = re.search('n(\d+)\.txt', match)
            self.data[int(n.group(1))] = np.loadtxt(match)

    def compute_analytic_solution(self):
        x = np.linspace(0, 1, 1000)
        y = 1 - (1-np.exp(-10))*x - np.exp(-10*x)
        self.analytic = (x, y)

    def plot(self):
        fig, ax = plt.subplots()
        for n, data in self.data.items():
            x = np.linspace(0, 1, n+2)
            ax.plot(x, data, label='n = {}'.format(n))
        ax.plot(*self.analytic, label='analytic')
        ax.legend()
        plt.show()


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Analyzes data for project 1')
    parser.add_argument('search_path', type=os.path.abspath,
                        help="Directory to search for input files")
    args = parser.parse_args()
    analyzer = Analyzer(args.search_path)
    analyzer.plot()
