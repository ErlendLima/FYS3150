#!/bin/python3
# -*- coding: utf-8 -*-

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import seaborn as sns
import argparse
import os
import re

sns.set(context="poster")


class Analyzer:
    def __init__(self, path, evolution_bin, energy_bin, magnetic_bin):
        self.evolution = self.load_evolution(os.path.join(path, evolution_bin))
        self.energy = self.load_binary(os.path.join(path, energy_bin))
        self.magnetic = self.load_binary(os.path.join(path, magnetic_bin))

    def load_evolution(self, path):
        M, I, J = [int(i) for i in re.findall(".*?(\d+)by(\d+)by(\d+)\.bin", path)[0]]
        data = np.fromfile(path, dtype=np.int8)
        return data.reshape(M, I, J)

    def load_binary(self, path):
        return np.fromfile(path)

    def plot(self):
        self.plot_expectations()

    def plot_inital(self):
        initial = self.evolution[0, :, :]
        plt.matshow(initial)
        plt.show()

    def plot_expectations(self):
        fig = plt.figure(figsize=(9, 7))
        ax_energy, ax_magnetic = fig.subplots(1, 2, sharex=True)
        ax_energy.plot(self.energy)
        ax_magnetic.plot(self.magnetic)
        plt.show()

    def update_lines(self, num, plot, text):
        plot.set_data(self.evolution[num, :, :])
        text.set_text(num)
        return [plot, text]

    def animate(self, save=False):
        fig = plt.figure()
        # Ax fucks up with fignum. Bug in matplotlib?
        plot = plt.matshow(self.evolution[0, :, :], fignum=0)
        text = plt.text(0.05, 0.9, '0', bbox={'facecolor':'white', 'alpha': 0.5, 'pad': 5})
        plt.grid(False)

        system_animation = animation.FuncAnimation(fig, self.update_lines,
                                                   fargs=(plot, text),
                                                   frames=range(0, self.evolution.shape[0], 100),
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
    parser.add_argument('--evolution', type=str, default="evolution50000by20by20.bin",
                        help="The file containing the positions")
    parser.add_argument('--energy', type=str, default="energies50000.bin",
                        help="The file containing the energies")
    parser.add_argument('--magnetic', type=str, default="magneticmoment50000.bin",
                        help="The file containing the angular momenta")
    parser.add_argument('--plot2d', help="Plot as 2D",
                        action="store_true")
    parser.add_argument('--animate', help="Animate the orbits",
                        action="store_true")
    parser.add_argument('--save', help="Save the (animation) plots",
                        action="store_true")
    args = parser.parse_args()
    analyzer = Analyzer(args.search_path, args.evolution,
                        args.energy, args.magnetic)
    if args.animate:
        analyzer.animate(args.save)
    else:
        analyzer.plot()
