#!/bin/python3
# -*- coding: utf-8 -*-

import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import seaborn as sns
import argparse
import os

sns.set()


class Analyzer:
    def __init__(self, path, name):
        self.data = self.load(path, name)

    def load(self, path, name):
        path = os.path.join(path, name)
        data = np.loadtxt(path)
        N, M = data.shape
        return data.reshape(N, 3, M//3, order='F')

    def plot(self, savename, K=3):
        fig = plt.figure()
        ax = fig.add_subplot(111, projection='3d')
        ax.scatter(0,0,0, marker='o',c=(1,0.6,0),s=200)
        for n in range(self.data.shape[2]):
            ax.plot(*self.data[:, :, n].T)
            # ax.plot(xs=self.data[:, 0, n],
            #         ys=self.data[:, 1, n],
            #         zs=self.data[:, 2, n])
        ax.legend()
        ax.set_xlabel(r'$x [m]$')
        ax.set_ylabel(r'$y [m]$')
        plt.show()
        fig.savefig('../latex/figures/{}.eps'.format(savename),
                    dpi=1200)
        fig.savefig('../latex/figures/{}.png'.format(savename),
                    dpi=1200)



if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Analyzes data for project 3')
    parser.add_argument('--search_path', type=os.path.abspath,
                        default='../data/',
                        help="Directory to search for input files")
    parser.add_argument('filename', type=str,
                        help="File to be analyzed")
    args = parser.parse_args()
    analyzer = Analyzer(args.search_path, args.filename)
    analyzer.plot('solarsys')
