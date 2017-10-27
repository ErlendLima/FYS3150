#!/bin/python3
# -*- coding: utf-8 -*-
from runner import Runner
import numpy as np
import matplotlib.pyplot as plt


class Perihelion(Runner):
    def setup(self):
        self['number of years'] = 100
        self['do save results'] = True
        self['do save any results'] = False
        self['use all planets'] = False
        self['save period'] = 1000
        self['use planets'] = ['Sun', 'Mercury']
        self['steps per year'] = 1e7
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
        # self.run_analysis()
        self.getPerihelion()

    def getPerihelion(self):
        timenangle = np.loadtxt("../data/precession.txt");
        print(timenangle.shape)
        print(timenangle[:, 1] - timenangle[0, 1])
        plt.plot(timenangle[:, 0], timenangle[:, 1], 'o')
        plt.xlabel("Time [yr]")
        plt.ylabel("Precession angle")
        plt.show()

    def findPerihelion(self):
        position = self.get_position()
        r = np.linalg.norm(position[:, :, 1] - position[:, :, 0], axis=1)
        # plt.plot(r)
        # plt.show()
        # argrelextrema()
        rfunc = interp1d(np.linspace(0,len(r)-1,len(r)),r, kind='slinear')
        r = rfunc(np.linspace(0,len(r)-1, len(r)))
        larger = np.roll(r, 1) - r > 1e-3
        smaller = np.roll(r, -1) - r > 1e-3
        minima = np.logical_and(larger, smaller)
        above_mean = r < r.mean()
        minima = np.logical_and(minima, above_mean)
        plt.plot(r)
        plt.plot(r*minima, 'o')
        plt.show()

        print(minima.sum())
        x, y = position[minima, 0:2, 1].T
        print(x.shape, y.shape)
        theta = np.arctan2(y, x)
        plt.plot(theta*180/np.pi)
        plt.show()

if __name__ == '__main__':
    with Perihelion() as mercury:
        mercury.run()
