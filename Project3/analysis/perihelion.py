#!/bin/python3
# -*- coding: utf-8 -*-
from runner import Runner
import numpy as np
import matplotlib.pyplot as plt


class Perihelion(Runner):
    def setup(self):
        self['number of years'] = 10
        self['do save results'] = False
        self['do save any results'] = True
        self['use all planets'] = False
        self['save period'] = 10000
        self['use planets'] = ['Sun', 'Mercury']
        self['steps per year'] = 1e6
        self['freeze sun'] = True
        self['use two body approximation'] = False
        self['use relativistic correction'] = True
        self['method'] = 'verlet'
        self.get_planet('Sun')['position'] = [0.0, 0.0, 0.0]
        self.get_planet('Sun')['Velocity'] = [0.0, 0.0, 0.0]
        self.get_planet('Mercury')['velocity'] = [12.44, 0.0, 0.0]
        self.get_planet('Mercury')['position'] = [0.0, 0.3075, 0.0]

    def run(self):
        out, _ = self.run_simulation()
        print(out)
        self.run_analysis()
        self.getPerihelion()

    def getPerihelion(self):
        timenangle = np.loadtxt("../data/precession.txt");
        print(timenangle.shape)
        print(timenangle[:, 1] - timenangle[0, 1])
        plt.plot(timenangle[:, 0], timenangle[:, 1], 'o')
        plt.xlabel("Time [yr]")
        plt.ylabel("Precession angle")
        plt.show()


if __name__ == '__main__':
    with Perihelion() as mercury:
        mercury.run()
