#!/bin/python3
# -*- coding: utf-8 -*-

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import seaborn as sns
import argparse
import os
import json
import pandas as pd

sns.set(context="poster")


class Analyzer:
    def __init__(self, base_path, meta_path):
        self.load(base_path, meta_path)

        self.labels = {'Cv'       : r'Heat Capacity $C_V$',
                       'E'        : r'Energy $\langle E \rangle$',
                       'Mabs'     : r'$\langle |M| \rangle$',
                       'sus'      : r'Susceptibility $\chi$',
                       'T'        : r'Temperature',
                       'ESquared' : r'$\langle E^2 \rangle$',
                       'MSquared' : r'$\langle M^2 \rangle$',
                       'M'        : r'Magnetic Moment $\langle M \rangle$',
                       'varE'     : r'$V(E)$',
                       'varM'     : r'$V(M)$'}

    def load(self, base_path: str, meta_path: str) -> None:
        with open(os.path.join(base_path, meta_path)) as fp:
            meta = json.load(fp)

        if meta['parallel']:
            self.parallel = True
            self.expectation_values = pd.read_csv("../data/data.txt", sep=" ")
        else:
            self.energy = self.load_block(base_path, meta["energy"])
            self.magnetic = self.load_block(base_path, meta["magnetic moment"])
            self.evolution = self.load_block(base_path, meta["evolution"])

    @staticmethod
    def load_block(base_path: str, block: dict) -> np.ndarray:
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
        self.plot_expectations('Cv')
        self.plot_expectations('E')
        self.plot_expectations('Mabs')
        self.plot_expectations('sus')

    def plot_inital(self):
        initial = self.evolution[0, :, :]
        plt.matshow(initial)
        plt.show()

    def plot_expectations(self, key):
        fig = plt.figure()
        ax = fig.subplots()
        ax.plot(self.expectation_values['T'],
                self.expectation_values[key], label = "Numeric")

        if key == 'Cv':
            T_arr = np.linspace(self.expectation_values['T'][0],
                                self.expectation_values['T'].tail(1),
                                1001)
            inst = Analytic2x2(T_arr)
            ax.plot(T_arr, inst.CV_hyp()/4, label = "Analytic")

        ax.set_xlabel(self.labels['T'])
        ax.set_ylabel(self.labels[key])
        ax.legend()
        plt.tight_layout()
        plt.show()

    def plot_energy_magnetic_moment(self):
        fig = plt.figure(figsize=(9, 7))
        ax_energy, ax_magnetic = fig.subplots(2, 1, sharex=True)
        ax_energy.plot(self.energy)
        ax_energy.set_title("Energy")
        ax_magnetic.plot(self.magnetic)
        ax_magnetic.set_title("Magnetic moment")
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
                                                   frames=range(0, self.evolution.shape[0], 1),
                                                   interval=10, blit=False)
        if save:
            system_animation.save('../latex/figures/animation.gif',
                                  writer='imagemagick', fps=30)
        else:
            plt.show()

class Analytic2x2:
    def __init__(self, T):
        self.T = T
        self.beta = 1/T
        self.arg = 8*self.beta

    def partition_function(self):
        return 4*np.cosh(self.arg) + 12

    def energy(self):
        numer = 8*np.sinh(8*beta)
        div   = 4*np.cosh(self.arg) + 12
        return -numer/div

    def energy_squared(self):
        numer = 256*np.cosh(self.arg)
        div   = 4*np.cosh(self.arg) + 12
        return numer/div

    def abs_magnetization(self):
        numer = 2*np.exp(self.arg) + 4
        div   = np.cosh(self.arg) + 3
        return numer/div

    def magnetization_squared(self):
        numer = 8*np.exp(self.arg) + 4
        div   = np.cosh(self.arg) + 3
        return numer/div

    def heat_capacity(self):
        fac1 = 128*(np.exp(self.arg) + np.exp(-self.arg))
        fac2 = -16*np.exp(self.arg) + 16*np.exp(-self.arg)
        div  = 2*np.exp(self.arg) + 2*np.exp(-self.arg) + 12
        return self.beta**2 * ((fac1/div) - (fac2/div)**2)

    def CV_hyp(self):
        fac1 = 256*np.cosh(self.arg)
        fac2 = -32*np.sinh(self.arg)
        denom = 4*np.cosh(self.arg) + 12
        return self.beta**2 * ( (fac1/denom) - (fac2/denom)**2)

    def susceptibility(self):
        num = 32*np.exp(self.arg) + 32
        div = 2*np.exp(self.arg) + 2*np.exp(-self.arg) + 12
        return self.beta*num/div

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Analyzes data for project 3')
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
