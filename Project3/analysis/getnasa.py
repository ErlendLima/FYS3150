import telnetlib
import json
import re
import sys


class Planet:
    def __init__(self, name: str, mass: str,
                 X: str, Y: str, Z: str,
                 VX: str, VY: str, VZ: str) -> None:
        self.name = name
        self.mass = mass
        self.pos = [X, Y, Z]
        self.vel = [VX, VY, VZ]

    def to_dict(self) -> None:
        return {'name': self.name,
                'mass': self.mass,
                'position': self.pos,
                'velocity': self.vel}


def construct_planets(start: str, end: str, filename: str) -> [Planet]:
    planets = {}

    with open(filename, 'r') as ids:
        for line in ids:
            id = line.rstrip()
            print(f"Contacting NASA for ID{id}")
            planet = Planet(*get_info(id, start, end))
            planets[planet.name] = planet
            print(f"Constructed {planet.name}")

    # Scale each planet by the units of the sun
    # and distance convert to AU
    sunmass = planets['Sun'].mass
    for name, planet in planets.items():
        sys.stdout.write(f"Converting the mass of {name} from {planet.mass} to ")
        planet.mass = planet.mass / sunmass
        print(planet.mass)
        planet.vel = [x*365.242199 for x in planet.vel]

    return [planet for _, planet in planets.items()]


def get_info(ID: str, startdate: str, enddate: str) -> (str, str,
                                                        str, str, str,
                                                        str, str, str):
    interactions = (('Horizons>', str(ID) + ''),
                    ('Select', 'E'),
                    ('Observe', 'v'),
                    ('Coordinate center', '500@0'),
                    ('Confirm selected station.*>', 'y'),
                    ('Reference plane.', 'eclip'),
                    ('Starting', str(startdate)),
                    ('Ending', str(enddate)),
                    ('Output interval', '1d'),
                    ('Accept default output:', 'y'),
                    ('Scroll', ' '),
                    ('Select', 'x'))
    interactions = [(p.encode('ascii'), r.encode('ascii'))
                    for p, r in interactions]

    telnet = telnetlib.Telnet()
    telnet.open('horizons.jpl.nasa.gov', 6775)

    name_pattern = re.compile(r'Target body name: (\w+) ')
    mass_pattern = re.compile(r'Mass,?\s+(?:\w+)?\s*\(?\d+\^(\d+) .*?\)?.\s*[=~]\s*(\w+\.\w+)[\s\+]')
    position_pattern = re.compile(r'(?<!V)[XYZ]\s?=\s?(.+?)[\s\n]')
    velocity_pattern = re.compile(r'(?<=V)[XYZ]\s?=\s?(.+?)[\s\n]')
    data = ''
    for prompt, response in interactions:
        data += telnet.read_until(prompt, 5).decode('ascii')
        telnet.write(response + b"\r\r\n")
    try:
        name = name_pattern.search(data)[1]
        exponent, mass = mass_pattern.search(data).group(1, 2)
        X, Y, Z = [float(val) for val in position_pattern.findall(data)][:3]
        VX, VY, VZ = [float(val) for val in velocity_pattern.findall(data)][:3]
    except Exception as error:
        print("Failed to extract information from ", ID, data)
        raise(error)

    mass = float(mass) * 10**float(exponent)
    return name, mass, X, Y, Z, VX, VY, VZ


def dump_planets_to_file(planets: [Planet], filename: str) -> None:
    bodies = [p.to_dict() for p in planets]
    with open(filename, 'r') as rfile:
        data = json.load(rfile)
        data['planets'] = bodies
    with open(filename, 'w') as wfile:
        json.dump(data, wfile, indent=4)


if __name__ == '__main__':
    # The info is always retrieved from the startdate,
    # the end date is only given for the telnet site to work
    start = "1977-Oct-18"
    end = "1977-Oct-19"
    planets = construct_planets(start, end, filename="targets.txt")
    dump_planets_to_file(planets, filename="../data/parameters.json")
