#!/bin/python3
# -*- coding: utf-8 -*-
from runner import Runner
import numpy as np
from math import pi, sqrt
import matplotlib.pyplot as plt


class Escaper(Runner):
    def setup(self):
        self['number of years'] = 10000
        self['do save results'] = True
        self['use all planets'] = False
        self['use planets'] = ['Sun', 'Earth']
        self['steps per year'] = 1e2
        self['freeze sun'] = True
        self['use two body approximation'] = False
        self['method'] = 'verlet'
        self.get_planet('Earth')['velocity'][1] = 0.0
        self.get_planet('Earth')['velocity'][2] = 0.0
        self.get_planet('Earth')['position'][1] = 0.0
        self.get_planet('Earth')['position'][0] = 1.0
        self.get_planet('Earth')['position'][1] = 0.0
        self.get_planet('Earth')['position'][2] = 0.0

    def change_velocity(self, new_velocity) -> None:
        self.get_planet('Earth')['velocity'][0] = new_velocity

    def get_velocity(self):
        return self.get_planet('Earth')['velocity'][0]

    def get_dt(self):
        return 1/self['steps per year']

    def has_escaped(self):
        positions = self.get_position()
        # After a sufficient about of time, the sign of x changes.
        # If the sign does not change back again, the planet has escaped
        x = positions[:, 0, 1]
        v = (x[1:] - x[:-1])
        v = v < 0
        if np.any(v):
            return False
        return True

    def run(self):
        nmax = 3*pi
        nmin = 2
        N = nmin
        N_prev = nmax
        while abs(N-N_prev) > 1e-2:
            self.change_velocity(N)
            out, _ = self.run_simulation()
            print(f"Simulation ran in {self.extract_time(out)} s")
            print(f"Velocity is {N}")
            if self.has_escaped():
                print(f"Earth escaped with velocity = {N}")
                nmax = N
            else:
                nmin = N
                N_prev = N
            N = (nmax-nmin)/2 + nmin
        return N

    def find_convergence(self):
        velocities = []
        years = [1, 10, 100, 1000, 10_000]
        for year in years:
            print(f"Simulating for {year} years")
            self['number of years'] = year
            velocity = self.run()
            velocities.append(velocity)
        plt.semilogx(years, velocities, '-o', label='Numerical')
        plt.semilogx(years, np.ones_like(years) * 2*pi*sqrt(2), label='Theoretical')
        plt.xlabel('Simulation period [years]')
        plt.ylabel('Escape Velocity [AU/year]')
        plt.legend()
        plt.savefig('../latex/figures/escapevelocity.eps')
        plt.show()

    def change_gravity(self):
        betas = np.linspace(2, 3, 10)
        velocities = []
        self['number of years'] = 1e3
        for beta in betas:
            self['gravitational exponent'] = beta
            velocities.append(self.run())
        plt.plot(betas, velocities, '-o')
        plt.xlabel(r'$\beta$')
        plt.ylabel('Escape Velocity [AU/year]')
        plt.savefig('../latex/figures/escapevelocitybeta.eps')
        plt.show()


if __name__ == '__main__':
    with Escaper() as runner:
        runner.change_gravity()
