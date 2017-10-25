#!/bin/python3
# -*- coding: utf-8 -*-
from runner import Runner


class JupiterRunner(Runner):
    def setup(self):
        self['number of years'] = 12
        self['do save results'] = True
        self['use all planets'] = False
        self['use planets'] = ['Sun', 'Earth', 'Jupiter']
        self['steps per year'] = 1e3
        self['freeze sun'] = False
        self['use two body approximation'] = False
        self['method'] = 'verlet'

    def run(self):
        self.setup()
        for mass_multiplier in [1, 10, 1000]:
            self.get_planet('Jupiter')['mass'] *= mass_multiplier
            out, _ = self.run_simulation()
            print(f"Simulation ran in {self.extract_time(out)} s")
            self.run_analysis('--animate')

        self.revert_parameters()


if __name__ == '__main__':
    runner = JupiterRunner()
    runner.run()
