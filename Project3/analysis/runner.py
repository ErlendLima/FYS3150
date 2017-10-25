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
        self.analysis_path = 'analyzer.py'
        self.parameters = {}
        self.original_params = self.load_parameters()
        self.parameters = self.load_parameters()

    def __setitem__(self, *args):
        return self.parameters.__setitem__(*args)

    def __getitem__(self, *args):
        return self.parameters.__getitem__(*args)

    def run_simulation(self) -> str:
        self.save_parameters(self.parameters)
        process = subprocess.Popen(self.exe_path, stdout=subprocess.PIPE,
                                   stderr=subprocess.PIPE, shell=True)
        out, _ = process.communicate()
        out = out.decode('ascii')
        return out

    def run_analysis(self, arguments: str) -> None:
        args = shlex.split(arguments)
        process = subprocess.Popen(self.analysis_path, args=args,
                                   stdout=subprocess.PIPE,
                                   stderr=subprocess.PIPE, shell=True)
        out, _ = process.communicate()

    def get_energy(self) -> str:
        return np.loadtxt(self.energy_path)

    def get_position(self) -> str:
        position = np.loadtxt(self.position_path)
        M, N = position.shape
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
