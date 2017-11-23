#!/bin/python3
# -*- coding: utf-8 -*-

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from mpl_toolkits.mplot3d import Axes3D
import seaborn as sns
import matplotlib as mpl
import argparse
import os
from numpy import pi, sin, exp
import json
import pandas as pd

sns.set(context="poster", style='white')


class Analyzer:
    def __init__(self, base_path, meta_path):
        self.load(base_path, meta_path)

    def load(self, base_path: str, meta_path: str) -> None:
        with open(os.path.join(base_path, meta_path)) as fp:
            self.meta = json.load(fp)
            self.solution = self.load_block(base_path, self.meta["solution"])

    @staticmethod
    def load_block(base_path: str, block: dict) -> np.ndarray:
        if block["format"] == "arma_ascii":
            return np.loadtxt(os.path.join(base_path, block["path"]))

        if block["type"] == "float64":
            type = np.float64
        elif block["type"] == "int8":
            type = np.int8
        elif block["type"] == "int16":
            type = np.int16
        elif block["type"] == "int32":
            type = np.int32
        elif block["type"] == "int64":
            type = np.int64
        elif block["type"] == "uint64":
            type = np.uint64
        else:
            raise(RuntimeError(f"{block['type']} from {block['path']} is not supported"))

        data = np.fromfile(os.path.join(base_path, block["path"]), dtype=type)
        return data.reshape(*block["dim"])

    def plot(self):
        # self.plot_analytic_solution()
        self.plot_numerical_solution()

    def plot_numerical_solution(self):
        x = np.linspace(0, 1, self.meta['x steps']+2)
        trange = range(0, self.meta['t steps'], self.meta['t steps']//10)
        palette = sns.color_palette("GnBu_d", len(trange))
        for i, t in enumerate(trange):
            plt.plot(x, self.solution[t, :], label=rf'{t*self.meta["dt"]}', c=palette[i])
        plt.legend()
        plt.show()

    def plot_analytic_solution(self):
        U = 1
        h = 1
        nu = 1

        def u(y, t):
            S = 0
            for n in range(1, 1000):
                S += 1/n*exp(-n**2*pi**2*nu*t/h**2)*sin(n*pi*(1-y/h))
            return U*y/h - 2*U/pi * S

        fig, ax = plt.subplots(1)
        y = np.linspace(0, 1, 100)
        ts = [0, 0.001, 0.01, 0.1, 0.2, 0.3, 0.5, 0.8, 0.9, 1.0]
        palette = sns.color_palette("GnBu_d", len(ts))
        for i, t in enumerate(ts):
            ax.plot(y, u(y, t), label=rf'$u(y, {t})$', c=palette[i])
        ax.legend()
        plt.show()

    def update_lines(self, num, lines, scatter):
        start = 0 if num < 1000 else num-1000
        for index, (line, scatt) in enumerate(zip(lines, scatter)):
            line[0].set_data(*self.position[start:num+1, 0:2, index].T)
            line[0].set_3d_properties(self.position[start:num+1, 2, index])
            scatt[0].set_data(*self.position[num:num+1, 0:2, index].T)
            scatt[0].set_3d_properties(self.position[num:num+1, 2, index])
        return lines

    def animate(self, save=False):
        fig = plt.figure()
        ax = fig.add_subplot(111, projection='3d')
        # Set the limits of the axis to the greatest of all x-, y- and z-axes.
        xmin = np.min(self.position[:, 0, :])
        ymin = np.min(self.position[:, 1, :])
        zmin = np.min(self.position[:, 2, :])
        xmax = np.max(self.position[:, 0, :])
        ymax = np.max(self.position[:, 1, :])
        zmax = np.max(self.position[:, 2, :])
        axmax = max([xmax, ymax, zmax])
        axmin = min([xmin, ymin, zmin])
        for axlim in [ax.set_xlim3d, ax.set_ylim3d, ax.set_zlim3d]:
            axlim([axmin, axmax])

        ax.set_xlabel('X [AU]')
        ax.set_ylabel('Y [AU]')
        ax.set_zlabel('Z [AU]')
        ax.grid(False)

        lines = [ax.plot(*self.position[:1, :, n].T, alpha=0.5)
                 for n in range(self.position.shape[2])]
        scatter = [ax.plot(*self.position[:1, :, n].T, linestyle='', marker='o')
                   for n in range(self.position.shape[2])]

        system_animation = animation.FuncAnimation(fig, self.update_lines,
                                                   frames=range(0, self.position.shape[0], 500),
                                                   fargs=(lines, scatter,),
                                                   interval=10, blit=False)
        if save:
            system_animation.save('../latex/figures/animation.gif',
                                  writer='imagemagick', fps=30)
        else:
            plt.show()


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Analyzes data for project 5')
    parser.add_argument('--search_path', type=os.path.abspath,
                        default='../data/',
                        help="Directory to search for input files")
    parser.add_argument('--meta', type=str, default="meta.json",
                        help="The file containing the metadata")
    parser.add_argument('--plot2d', help="Plot as 2D",
                        action="store_true")
    parser.add_argument('--animate', help="Animate the orbits",
                        action="store_true")
    parser.add_argument('--save', help="Save the (animation) plots",
                        action="store_true")
    args = parser.parse_args()
    analyzer = Analyzer(args.search_path, args.meta)
    if args.animate:
        analyzer.animate(args.save)
    else:
        analyzer.plot()
