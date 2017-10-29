#!/bin/python3
# -*- coding: utf-8 -*-
from runner import Runner


class FullRunner(Runner):
    def setup(self):
        self['number of years'] = 300
        self['use all planets'] = False
        self['use planets'] = ['Sun', 'Earth', 'Jupiter', 'Venus', 'Mercury',
                               'Saturn', 'Neptune', 'Uranus', 'Pluto', 'Mars']
        self['steps per year'] = 1e5
        self['freeze sun'] = False
        self['use two body approximation'] = False
        self['method'] = 'verlet'
        self['do save results'] = False
        self['do save any results'] = True
        self['save period'] = 1000
        self['find perihelion precession'] = False
        self['use relativistic correction'] = False
        self['gravitational exponent'] = 2

    def run(self):
        out, _ = self.run_simulation()
        print(out)
        print(f"Simulation ran in {self.extract_time(out)} s")
        self.run_analysis()


if __name__ == '__main__':
    with FullRunner() as system:
        system.run()
