module Celestial
export CelestialBody, distance, init, tomatrix
include("vector3d.jl")
import Vector3D: Vec3, +, -, *, \, norm, tomatrix

mutable struct CelestialBody{T}
    pos::Vector{Vec3{T}}
    vel::Vector{Vec3{T}}
    mass::T
end

function CelestialBody(N::Number)
    pos = [Vec3(0.0) for i in 1:N]
    vel = [Vec3(0.0) for i in 1:N]
    CelestialBody(pos, vel, 0.0)
end

function init(body::CelestialBody{T}, pos::Vec3{T}, vel::Vec3{T}, mass::T) where {T}
    body.pos[1] = pos
    body.vel[1] = vel
    body.mass = mass
end

init(body, pos, vel, mass) = init(body, Vec3(pos), Vec3(vel), mass)

distance(self::CelestialBody, other::CelestialBody, n) = norm(self.pos[n]-other.pos[n])

function tomatrix(body::CelestialBody)
    output = zeros(length(body.pos), 3)
    for n in 1:length(body.pos)
        output[n, :] = tomatrix(body.pos[n])
    end
    output
end

end
