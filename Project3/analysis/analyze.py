#!/bin/python3
# -*- coding: utf-8 -*-

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from mpl_toolkits.mplot3d import Axes3D
import seaborn as sns
import argparse
import os

sns.set()


class Analyzer:
    def __init__(self, path, position, energy, plot2d=False):
        self.position = self.load(path, position)
        self.energy = self.load(path, energy)
        self.plot2d = plot2d

    def load(self, path, name):
        path = os.path.join(path, name)
        data = np.loadtxt(path)
        N, M = data.shape
        return data.reshape(N, 3, M//3, order='F')

    def plot(self, K=3):
        # Position
        fig = plt.figure()
        if not self.plot2d:
            ax = fig.add_subplot(111, projection='3d')
            ax.scatter(0, 0, 0, marker='o', c=(1, 0.6, 0), s=200)
            for n in range(self.position.shape[2]):
                ax.plot(*self.position[:, :, n].T)
        else:
            ax = fig.add_subplot(111)
            ax.scatter(0, 0, marker='o', c=(1, 0.6, 0), s=200)
            for n in range(self.position.shape[2]):
                ax.plot(*self.position[:, 0:2, n].T)
        ax.legend()
        ax.set_xlabel('X [AU]')
        ax.set_ylabel('Y [AU]')
        if not self.plot2d:
            ax.set_zlabel('Z [AU]')

        fig.savefig('../latex/figures/position.eps', dpi=1200)
        # Energy
        fig_energy, (kinetic, potential, total) = plt.subplots(3, sharex=True)
        kinetic.plot(*self.energy[0:2, :].T,
                     label="Kinetic")
        potential.plot(self.energy[0, :], self.energy[2, :],
                       label="Potential")
        total.plot(self.energy[0, :],
                   np.sum(self.energy[[1, 2], :], axis=0),
                   label="Toal energy")
        total.set_xlabel("Time [years]")
        plt.show()
        fig_energy.savefig('../latex/figures/energy.eps', dpi=1200)

    def update_lines(self, num, lines, scatter):
        for index, (line, scatt) in enumerate(zip(lines, scatter)):
            line[0].set_data(*self.position[0:num+1, 0:2, index].T)
            line[0].set_3d_properties(self.position[0:num+1, 2, index])
            scatt[0].set_data(*self.position[num:num+1, 0:2, index].T)
            scatt[0].set_3d_properties(self.position[num:num+1, 2, index])
        return lines

    def animate(self, save=False):
        fig = plt.figure()
        ax = fig.add_subplot(111, projection='3d')
        xmin = np.min(self.position[:, 0, :])
        ymin = np.min(self.position[:, 1, :])
        zmin = np.min(self.position[:, 2, :])
        xmax = np.max(self.position[:, 0, :])
        ymax = np.max(self.position[:, 1, :])
        zmax = np.max(self.position[:, 2, :])
        ax.set_xlim3d([xmin, xmax])
        ax.set_xlabel('X [AU]')
        ax.set_ylim3d([ymin, ymax])
        ax.set_ylabel('Y [AU]')
        ax.set_zlim3d([zmin, zmax])
        ax.set_zlabel('Z [AU]')
        ax.grid(False)

        lines = [ax.plot(*self.position[:1, :, n].T, alpha=0.5)
                 for n in range(self.position.shape[2])]
        scatter = [ax.plot(*self.position[:1, :, n].T, linestyle='', marker='o')
                   for n in range(self.position.shape[2])]

        system_animation = animation.FuncAnimation(fig, self.update_lines,
                                                   frames=range(0, self.position.shape[0], 10),
                                                   fargs=(lines, scatter,),
                                                   interval=10, blit=False)
        if save:
            system_animation.save('../latex/figures/animation.gif', writer='imagemagick', fps=30)
        else:
            plt.show()



if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Analyzes data for project 3')
    parser.add_argument('--search_path', type=os.path.abspath,
                        default='../data/',
                        help="Directory to search for input files")
    parser.add_argument('position_filename', type=str,
                        help="The file containing the positions")
    parser.add_argument('energy_filename', type=str,
                        help="The file containing the energies")
    parser.add_argument('--plot2d', help="Plot as 2D",
                        action="store_true")
    parser.add_argument('--animate', help="Animate the orbits",
                        action="store_true")
    parser.add_argument('--save', help="Save the (animation) plots",
                        action="store_true")
    args = parser.parse_args()
    analyzer = Analyzer(args.search_path, args.position_filename,
                        args.energy_filename,
                        plot2d=args.plot2d)
    if args.animate:
        analyzer.animate(args.save)
    else:
        analyzer.plot('solarsys')
