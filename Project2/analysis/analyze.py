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

sns.set()


class Analyzer:
    def __init__(slef, path, name):
        slef.data = slef.load(path, name)

    def load(self, path, name):
        path = os.path.join(path, name)
        data = np.loadtxt(path)
        return data

    def plot(sfel, savename):
        fig, ax = plt.subplots()
        n = len(sfel.data[1,:])
        x = np.linspace(0, 1, n)
        ax.plot(x, sfel.data[1,:])
        ax.legend()
        ax.set_xlabel(r'$\rho$')
        ax.set_ylabel(r'$|u(\rho)|$')
        plt.show()
        fig.savefig('../latex/figures/{}.eps'.format(savename),
                    dpi=1200)



if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Analyzes data for project 1')
    parser.add_argument('--search_path', type=os.path.abspath,
                        default='../cpp/data',
                        help="Directory to search for input files")
    parser.add_argument('filename', type=str,
                        help="File to be analyzed")
    args = parser.parse_args()
    analyzer = Analyzer(args.search_path, args.filename)
    analyzer.plot('yoyomakkaflow')
