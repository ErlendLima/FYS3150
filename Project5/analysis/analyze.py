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

    def get_xrange(self):
        if not hasattr(self, 'xrange'):
            self.xrange = np.linspace(self.meta['x start'], self.meta['x end'],
                                      self.meta['x steps']+2)
        return self.xrange

    def get_trange(self):
        if not hasattr(self, 'trange'):
            self.trange = np.linspace(self.meta['t start'], self.meta['t end'],
                                      self.meta['t steps'])
        return self.trange

    def plot(self):
        self.plot_analytic_solution()
        plt.show()
        # plt.savefig('../latex/figures/couette_numeric_vs_analytical.eps')
        self.plot_total_abs_error()
        plt.show()
        # plt.savefig('../latex/figures/couette_absolute_error.eps')
        self.plot_total_rel_error()
        plt.show()
        # plt.savefig('../latex/figures/couette_relative_error.eps')

    def plot_numerical_solution(self):
        trange = range(0, self.meta['t steps'], self.meta['t steps']//10)
        palette = sns.color_palette("GnBu_d", len(trange))
        for i, t in enumerate(trange):
            plt.plot(self.get_xrange(), self.solution[t, :], label=rf'{t*self.meta["dt"]}', c=palette[i])
        plt.legend()

    def plot_total_abs_error(self):
        fig, ax = plt.subplots(1, figsize=(7, 7))
        ax.plot(*self.calculate_total_abs_err())
        ax.set_xlabel("Time [s]")
        ax.set_ylabel("Total Absolute Error")

    def plot_total_rel_error(self):
        fig, ax = plt.subplots(1, figsize=(7, 7))
        ax.plot(*self.calculate_total_rel_err())
        ax.set_xlabel("Time [s]")
        ax.set_ylabel("Total Relative Error")

    def plot_analytic_solution(self):
        fig, ax = plt.subplots(1, figsize=(7, 7))
        y = self.get_xrange()
        N = 500
        ts = self.get_trange()[::N]
        palette = sns.color_palette("GnBu_d", len(ts))
        for i, t in enumerate(ts):
            ax.plot(y, self.analytical_fn(y, t), '--', label=rf'$u(y, {t})$', c=palette[i])
            ax.plot(y, self.solution[i*N, :], c=palette[i])
        ax.set_xlabel('x')
        ax.set_ylabel('y')
        # ax.legend()

    def animate_simple(self):
        y = np.linspace(0, 1, self.meta['x steps'] + 2)
        fig, ax = plt.subplots(1, 1)
        U = self.solution[0, :]
        zeros = np.zeros_like(U)
        ax.plot([0, 1], [1, 1], 'k-', alpha=0.5)
        ax.plot([0, 1], [0, 0], 'k-', alpha=0.5)
        Q = ax.quiver(zeros, y, U, zeros, scale=1)
        ax.set_xlim(0, 1.1*U.max())
        ax.set_ylim(0, 1.1)

        anim = animation.FuncAnimation(fig, self.update_quiver_simple,
                                       fargs=(U, Q), interval=10,
                                       frames=range(0, len(self.solution)),
                                       blit=False)
        plt.show()

    def update_quiver_simple(self, num, U, Q):
        U = self.solution[num, :]
        Q.set_UVC(U, np.zeros_like(U))
        return Q,

    def analytical_fn(self, y, t):
        init_cond = self.meta['initial condition']
        if init_cond == "zero":
            return self.analytical_zero(y, t)
        elif init_cond == "sin":
            return self.analytical_sin_2(y, t)

    def analytical_zero(self, y, t):
        U = 1
        h = 1
        nu = 1
        S = 0
        for n in range(1, 2000):
            S += 1/n*exp(-n**2*pi**2*nu*t/h**2)*sin(n*pi*(1-y/h))
        return U*y/h - 2*U/pi * S

    def analytical_sin(self, y, t):
        L = 1
        S = 0
        for n in range(2, 1000):
            S += sin(pi*n)/(1-n**2)*sin(pi*n*y/L)*exp(-n**2*pi**2*t/L**2)
        return 2/pi * S

    def analytical_sin_2(self, y, t):
        return exp(-pi**2*t)*sin(pi*y)


    def update_lines(self, num, lines, text):
        lines[0][0].set_data(self.get_xrange(), self.solution[num, :])
        text.set_text(f't = {self.get_trange()[num]}')
        return lines

    def animate(self, save=False):
        fig = plt.figure()
        ax = fig.add_subplot(111)
        xmin = np.min(self.get_xrange())
        ymin = np.min(self.solution)
        xmax = np.max(self.get_xrange())
        ymax = np.max(self.solution)
        ax.set_xlim([xmin, xmax])
        ax.set_ylim([ymin, ymax])

        ax.set_xlabel('x')
        ax.set_ylabel('u(t, x)')

        lines = [ax.plot(self.get_xrange(), self.solution[0, :])]
        text = ax.text(0.05, 0.9, 't = 0.0', bbox={'facecolor': 'white',
                                                   'alpha': 0.5,
                                                   'pad': 5})

        frames = range(0, self.meta['t steps']//4, self.meta['t steps']//100),
        system_animation = animation.FuncAnimation(fig, self.update_lines,
                                                   frames=frames,
                                                   fargs=(lines, text),
                                                   interval=10, blit=False)
        if save:
            system_animation.save('../latex/figures/animation.gif',
                                  writer='imagemagick', fps=30)
        else:
            plt.show()

    def update_quiver(self, num, Q, X, Y, text, indices, N, M):
        U = np.repeat(self.solution[num, indices], M).reshape((N+1, M))
        V = Y*0

        Q.set_UVC(U, V)
        self.X = self.X+U*0.01
        Q.set_offsets(self.XY_to_offsets(self.X, Y))
        text.set_text(f't = {self.get_trange()[num]:.3f}')
        return Q,

    def XY_to_offsets(self, X, Y):
        Z = []
        for i in range(len(X)):
            for j in range(len(X[i])):
                Z.append([X[i][j], Y[i][j]])
        return np.array(Z)

    def animate_fancy(self, save=False):
        fig = plt.figure()
        ax = fig.add_subplot(111)
        xmin = np.min(self.get_xrange())
        ymin = np.min(self.solution)
        xmax = np.max(self.get_xrange())
        ymax = np.max(self.solution)
        ax.set_xlim([0, 1])
        ax.set_ylim([xmin-0.2, xmax+0.2])

        ax.set_xlabel('x')
        ax.set_ylabel('y')
        ax.plot([0, 1], [xmax, xmax], 'k-', alpha=0.5)
        ax.plot([0, 1], [xmin, xmin], 'k-', alpha=0.5)
        ax.text((xmax-xmin)/2, xmax+0.1, r'$\vec{U}\longrightarrow$')

        N = 20
        M = 150
        yrange = self.get_xrange()
        xrange = np.linspace(-20, 1, M)
        indices = range(0, len(yrange), len(yrange)//N)

        X, Y = np.meshgrid(xrange, yrange[indices])
        self.X = X
        # U = np.repeat(self.solution[0, indices], N).reshape(M+1, N)
        U = np.ones_like(X)*0.3
        V = Y*0
        Q = ax.quiver(X, Y, U, V, units='inches', scale=2, width=0.015)
        text = ax.text(0.05, 1.1, 't = 0.0', bbox={'facecolor': 'white',
                                                   'alpha': 0.5,
                                                   'pad': 5})


        anim = animation.FuncAnimation(fig, self.update_quiver,
                                       frames=range(0, self.meta['t steps'], self.meta['t steps']//1000),
                                       fargs=(Q, X, Y, text, indices, N, M),
                                       interval=10, blit=False)
        if save:
            anim.save('../latex/figures/animation.gif',
                      writer='imagemagick', fps=30)
        else:
            plt.show()

    def calculate_rel_err(self, tlimits, method=np.max,
                          xlimits=[1, -1]):
        # Do not use the endpoints to avoid division by zero
        xstart, xend = xlimits

        # Get the ranges for y and t used
        t = self.get_trange()
        y = self.get_xrange()[xstart:xend]

        tmin, tmax = tlimits
        tstart = np.where(t >= tmax)[0][0]
        tend = np.where(t <= tmin)[0][0]
        # Get the analytical values for u(y, t)
        # It must be transposed since the solution has the shape u(t, y)

        # Calculate the relative error
        errors = []
        for index in [tstart, tend]:
            analytical = self.analytical_fn(y, t[index])
            rel_err = abs((analytical - self.solution[index, xstart:xend]) / analytical)
            errors.append(rel_err)

        # Return the time steps used and total error
        return y, errors

    def calculate_total_abs_err(self, method=np.sum,
                                tlimits=[1, -1000], xlimits=[1, -1]):
        t = self.get_trange()
        y = self.get_xrange()
        Y = np.repeat(y, len(t))
        y = np.reshape(Y, self.solution.shape)
        analytical = self.analytical_fn(y.T, t).T
        diff = method(abs(analytical-self.solution), axis=1)
        return t, diff

    def calculate_total_rel_err(self, method=np.max,
                                tlimits=[1, -1000], xlimits=[1, -1],
                                min_time=None, max_time=None):
        # Get the ranges for y and t used
        t = self.get_trange()
        y = self.get_xrange()

        # Repeat y so that it has the same shape as u(y, t)
        Y = np.repeat(y, len(t))
        y = np.reshape(Y, self.solution.shape[::-1])

        # Get the analytical values for u(y, t)
        # It must be transposed since the solution has the shape u(t, y)
        analytical = self.analytical_fn(y, t).T

        # Do not use the endpoints to avoid division by zero
        xstart, xend = xlimits
        tstart, tend = tlimits
        if max_time is not None:
            tend = np.where(t >= max_time)[0][0]
        if min_time is not None:
            tstart = np.where(t <= min_time)[0][0]

        # Calculate the relative error
        rel_err = abs((analytical[tstart:tend, xstart:xend] -
                      self.solution[tstart:tend, xstart:xend]) /
                      analytical[tstart:tend, xstart:xend])

        # Find either the sum or the max error in each time step
        diff = method(rel_err, axis=1)

        # Return the time steps used and total error
        return t[tstart:tend], diff


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Analyzes data for project 5')
    parser.add_argument('--search_path', type=os.path.abspath,
                        default='../data/',
                        help="Directory to search for input files")
    parser.add_argument('--meta', type=str, default="meta.json",
                        help="The file containing the metadata")
    parser.add_argument('--plot2d', help="Plot as 2D",
                        action="store_true")
    parser.add_argument('--animate', help="Animate the lines",
                        action="store_true")
    parser.add_argument('--animatef', help="A fancy animation",
                        action="store_true")
    parser.add_argument('--save', help="Save the (animation) plots",
                        action="store_true")
    args = parser.parse_args()
    analyzer = Analyzer(args.search_path, args.meta)
    if args.animate:
        analyzer.animate(args.save)
    elif args.animatef:
        analyzer.animate_fancy(args.save)
    else:
        analyzer.plot()
