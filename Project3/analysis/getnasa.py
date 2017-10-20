import telnetlib
import numpy as np
import re


class planetInfo:
    def printInfoToFile(self, startdate, enddate, filename):

        IDs = np.loadtxt(filename)
        print(f"Getting info for {len(IDs)} planets")

        kg_to_MS = 5.02739933e-31

        data = np.zeros((len(IDs), 2, 3))
        names = []
        masses = []
        IDs = np.array(IDs)
        for i in range(IDs.size):
            name, mass, data[i, :, :] = self.getInfo(
                int(IDs[i]), startdate, enddate)
            names.append(name)
            masses.append(mass)
            print(f"Found info for {name}")

        # Normalizes all the masses with the current mass of the sun(if the suns mass is retrived from NASA)
        if (names[0] == "Sun"):
            kg_to_MS = 1.0 / (masses[0])
        data_ = data.reshape(len(names), 6)

        with open("planetData.txt", "w") as outFile:
            for i in range(len(names)):
                outFile.write(str(names[i]) + "  ")
                for j in range(6):
                    outFile.write(str(data_[i, j]) + " ")
                outFile.write(str(masses[i] * kg_to_MS) + "\n")

        with open("planetConfig_" + str(len(names)) + ".txt", "w") as outFile:
            outFile.write(str(len(names)) + "\n")
            for name in names:
                outFile.write(name + "\n")

class Planet:
    def __init__(self, name, mass, X, Y, Z, VX, VY, VZ):
        self.name = name
        self.mass = mass
        self.pos = [X, Y, Z]
        self.vel = [VX, VY, VZ]


def construct_planets(start, end, filename):
    planets = {}

    with open(filename, 'r') as ids:
        for line in ids:
            id = line.rstrip()
            print(f"Contacting NASA for ID{id}")
            planet = Planet(*get_info(id, start, end))
            planets[planet.name] = planet
            print(f"Constructed {planets[-1].name}")

    # Scale each planet by the units of the sun
    for name, planet in planets.items():
        pass


def get_info(ID, startdate, enddate):
    interactions = ((r'Horizons>', str(ID) + ''),
                    (r'Select.*E.phemeris.*:', 'E'),
                    (r'Observe.*:', 'v'),
                    (r'Coordinate center.*:', '500@0'),
                    (r'Confirm selected station.*>', 'y'),
                    (r'Reference plane.*:', 'eclip'),
                    (r'Starting .* :', str(startdate)),
                    (r'Ending .* :', str(enddate)),
                    (r'Output interval.*:', '1d'),
                    (r'Accept default output.* :', 'y'),
                    (r'Scroll . Page: .*%', ' '),
                    (r'Select\.\.\. .A.gain.* :', 'x'))
    interactions = [(p.encode('ascii'), r.encode('ascii'))
                    for p, r in interactions]

    telnet = telnetlib.Telnet()
    telnet.open('horizons.jpl.nasa.gov', 6775)

    name_pattern = re.compile(r'Target body name: (\w+) ')
    mass_pattern = re.compile(r'Mass,?\s+\(\d+\^(\d+) .*?\).\s*[=~]\s*(.*?)\s')
    position_pattern = re.compile(r'(?<!V)[XYZ]\s?=\s?(.+?)[\s\n]')
    velocity_pattern = re.compile(r'(?<=V)[XYZ]\s?=\s?(.+?)[\s\n]')
    data = ''
    for prompt, response in interactions:
        data += telnet.read_until(prompt, 3).decode('ascii')
        telnet.write(response + b"\r\r\n")
    name = name_pattern.search(data)[1]
    exponent, mass = mass_pattern.search(data).group(1, 2)
    X, Y, Z = [float(val) for val in position_pattern.findall(data)][:3]
    VX, VY, VZ = [float(val) for val in velocity_pattern.findall(data)][:3]
    mass = float(mass) * 10**float(exponent)
    return name, mass, X, Y, Z, VX, VY, VZ


# The info is always retrieved from the startdate, the end date is only given for the telnet site to work
start = "1977-Oct-18"
end = "1977-Oct-19"


construct_planets(start, end, filename="targets.txt")
