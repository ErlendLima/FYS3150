#!/bin/python3
# -*- coding: utf-8 -*-

import subprocess
import json
import numpy as np
import seaborn as sns
import shlex
sns.set(context='poster')


class Runner:

    def __init__(self):
        self.param_path = '../data/parameters.json'
        self.exe_path = '../cpp/solve'
        self.energy_path = '../data/energy.txt'
        self.position_path = '../data/position.txt'
        self.analysis_path = 'analyze.py'
        self.original_params = self.load_parameters()
        self.parameters = self.load_parameters()

    def __enter__(self):
        self.setup()
        return self

    def __exit__(self, type, value, traceback):
        self.revert_parameters()

    def __setitem__(self, *args):
        return self.parameters.__setitem__(*args)

    def __getitem__(self, *args):
        return self.parameters.__getitem__(*args)

    def get_planet(self, name):
        for planet in self['planets']:
            if name == planet['name']:
                return planet
        raise IndexError(f"{planet} does not exist")

    def run_simulation(self) -> str:
        self.save_parameters(self.parameters)
        process = subprocess.Popen(self.exe_path, stdout=subprocess.PIPE,
                                   stderr=subprocess.PIPE, shell=True)
        out, err = process.communicate()
        return out.decode('ascii'), err.decode('ascii')

    @staticmethod
    def extract_time(out: str) -> str:
        time = float(out.split()[-1][:-1])  # Ugly ass code. Regex doesn't work
        return time

    def run_analysis(self, arguments: str = '') -> None:
        args = f"python {self.analysis_path} {arguments}"
        args = shlex.split(args)
        process = subprocess.run(args)

    def get_energy(self) -> str:
        return np.loadtxt(self.energy_path)

    def get_position(self) -> str:
        position = np.loadtxt(self.position_path)
        N, M = position.shape
        return position.reshape(N, 3, M//3, order='F')

    def revert_parameters(self) -> None:
        self.save_parameters(self.original_params)

    def load_parameters(self) -> str:
        with open(self.param_path, 'r') as param_file:
            param_json = json.load(param_file)
        return param_json

    def save_parameters(self, param_json: str) -> None:
        with open(self.param_path, 'w') as param_file:
            json.dump(param_json, param_file, indent=4)
