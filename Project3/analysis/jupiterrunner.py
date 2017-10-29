#!/bin/python3
# -*- coding: utf-8 -*-
from runner import Runner
from math import sqrt
import numpy as np


class JupiterRunner(Runner):
    def setup(self):
        self['number of years'] = 15
        self['use all planets'] = False
        self['use planets'] = ['Sun', 'Earth', 'Jupiter']
        self['steps per year'] = 5e5
        self['freeze sun'] = False
        self['use two body approximation'] = False
        self['method'] = 'verlet'
        self['do save results'] = False
        self['do save any results'] = True
        self['save period'] = 1000
        self['find perihelion precession'] = False
        self['use relativistic correction'] = False
        self['gravitational exponent'] = 2
        self.project_positions()

    def project_positions(self):
        """ Put the bodies into the x-y plane and make the total angular moment 0"""
        jvel = np.array(self.get_planet("Jupiter")["velocity"][:2])
        jpos = np.array(self.get_planet("Jupiter")["position"][:2])
        jmass = self.get_planet("Jupiter")["mass"]
        jmoment = jvel*jmass
        self.get_planet("Jupiter")["position"][2] = 0.0
        self.get_planet("Jupiter")["velocity"][2] = 0.0

        evel = np.array(self.get_planet("Earth")["velocity"][:2])
        epos = np.array(self.get_planet("Earth")["position"][:2])
        emass = self.get_planet("Earth")["mass"]
        emoment = evel*emass
        self.get_planet("Earth")["position"][2] = 0.0
        self.get_planet("Earth")["velocity"][2] = 0.0

        spos = self.get_planet("Sun")["position"][:2]
        smass = self.get_planet("Sun")["mass"]
        svelx = -(jmoment + emoment)/smass

        self.get_planet("Sun")["velocity"] = [svelx[0], svelx[1], 0.0]

    def run(self):
        mass = self.get_planet('Jupiter')['mass']
        for mass_multiplier in [1, 10, 1000]:
            self.get_planet('Jupiter')['mass'] = mass_multiplier*mass
            out, _ = self.run_simulation()
            print(out)
            print(f"Simulation ran in {self.extract_time(out)} s")
            self.run_analysis('--plot2d')


if __name__ == '__main__':
    with JupiterRunner() as threeBodyProblem:
        threeBodyProblem.run()
