#!/bin/python3
# -*- coding: utf-8 -*-

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from mpl_toolkits.mplot3d import Axes3D
import seaborn as sns
import argparse
import os

sns.set(context="poster")


class Analyzer:
    def __init__(self, path, position, energy, angular_momentum, plot2d=False):
        self.position = self.load(path, position)
        N, M = self.position.shape
        self.position = self.position.reshape(N, 3, M//3, order='F')

        self.energy = self.load(path, energy)
        self.angular_momentum = self.load(path, angular_momentum)
        self.plot2d = plot2d

    def load(self, path, name):
        path = os.path.join(path, name)
        data = np.loadtxt(path)
        return data

    def plot(self):
        self.plot_position()
        self.plot_angular_momentum()
        self.plot_energy()

    def plot_position(self):
        fig = plt.figure(figsize=(9, 7))
        if not self.plot2d:
            ax = fig.add_subplot(111, projection='3d')
            ax.scatter(*self.position[0, :, 0], marker='o',
                       c=(1, 0.6, 0), s=200)
            for n in range(self.position.shape[2]):
                ax.plot(*self.position[:, :, n].T)
        else:
            ax = fig.add_subplot(111)
            ax.scatter(*self.position[0, 0:2, 0], marker='o',
                       c=(1, 0.6, 0), s=200)
            for n in range(self.position.shape[2]):
                ax.plot(*self.position[:, 0:2, n].T)
        # ax.legend()
        ax.set_xlabel('X [AU]')
        ax.set_ylabel('Y [AU]')
        if not self.plot2d:
            ax.set_zlabel('Z [AU]')
        else:
            ax.axis('equal')

        fig.savefig('../latex/figures/position.eps', dpi=1200)

    def plot_angular_momentum(self):
        fig = plt.figure(figsize=(9, 7))
        ax = fig.add_subplot(111)
        time = self.energy[0, :]
        ax.plot(time, self.angular_momentum)
        ax.set_xlabel(r"Time [yr]")
        ax.set_ylabel(r"Total Angular Momentum [AU$^2$/yr]")
        plt.show()

    def plot_energy(self):
        fig_energy, (kinetic, potential, total) = plt.subplots(3, sharex=True,
                                                               figsize=(11, 8))
        self.energy = self.energy[:, 1:-2]
        # Time
        time = self.energy[0, :]
        # Kinetic Energy
        Ek = self.energy[1, :]/self.energy[1, 0]
        kinetic.plot(time, Ek)
        # Potential Energy
        Ep = self.energy[2, :]/self.energy[2, 0]
        potential.plot(time, Ep)
        # Total Energy
        Et = np.sum(self.energy[[1, 2], :], axis=0)/np.sum(self.energy[[1, 2], 0], axis=0)
        total.plot(time, Et)
        total.set_xlabel("Time [years]")
        kinetic.set_title("Kinetic Energy")
        potential.set_title("Potential Energy")
        total.set_title("Total Energy")
        plt.show()
        fig_energy.savefig('../latex/figures/energy.eps', dpi=1200)

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
    parser = argparse.ArgumentParser(description='Analyzes data for project 3')
    parser.add_argument('--search_path', type=os.path.abspath,
                        default='../data/',
                        help="Directory to search for input files")
    parser.add_argument('--position', type=str, default="position.txt",
                        help="The file containing the positions")
    parser.add_argument('--energy', type=str, default="energy.txt",
                        help="The file containing the energies")
    parser.add_argument('--angular', type=str, default="angmom.txt",
                        help="The file containing the angular momenta")
    parser.add_argument('--plot2d', help="Plot as 2D",
                        action="store_true")
    parser.add_argument('--animate', help="Animate the orbits",
                        action="store_true")
    parser.add_argument('--save', help="Save the (animation) plots",
                        action="store_true")
    args = parser.parse_args()
    analyzer = Analyzer(args.search_path, args.position,
                        args.energy, args.angular,
                        plot2d=args.plot2d)
    if args.animate:
        analyzer.animate(args.save)
    else:
        analyzer.plot()
