#!/bin/python3
# -*- coding: utf-8 -*-
from runner import Runner
import numpy as np
import matplotlib.pyplot as plt


class Perihelion(Runner):
    def setup(self):
        self['number of years'] = 100
        self['do save results'] = False
        self['do save any results'] = False
        self['use all planets'] = False
        self['save period'] = 10000
        self['use planets'] = ['Sun', 'Mercury']
        self['steps per year'] = 1e7
        self['freeze sun'] = True
        self['use two body approximation'] = False
        self['use relativistic correction'] = False
        self['method'] = 'verlet'
        self.get_planet('Sun')['position'] = [0.0, 0.0, 0.0]
        self.get_planet('Sun')['Velocity'] = [0.0, 0.0, 0.0]
        self.get_planet('Mercury')['velocity'] = [0.0, 12.4415373, 0.0]
        self.get_planet('Mercury')['position'] = [0.307491008, 0.0, 0.0]

    def run(self):
        out, _ = self.run_simulation()
        print(out)
        self.getPerihelion()

    def getPerihelion(self):
        timenangle = np.loadtxt("../data/precession.txt");
        fig = plt.figure(figsize=(11, 8))
        ax = fig.add_subplot(111)
        relative = timenangle[:, 1]-timenangle[0, 1]
        ax.plot(timenangle[:, 0], relative)
        ax.set_xlabel("Time [yr]")
        ax.set_ylabel("Precession angle relative to 0")
        print("Endpoint:", relative[-1], relative[-1]*180/np.pi * 3600)
        fig.savefig("../latex/figures/newtonprecession1.eps", dpi=1200)
        # plt.show()


if __name__ == '__main__':
    with Perihelion() as mercury:
        mercury.run()
