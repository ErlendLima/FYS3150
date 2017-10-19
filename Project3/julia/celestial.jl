module Celestial
export CelestialBody, distance, init!, tomatrix, show
include("vector3d.jl")
import Base: show
import Vector3D: Vec3, +, -, *, \, norm, tomatrix

mutable struct CelestialBody{T<:Real}
    pos::Vector{Vec3{T}}
    vel::Vector{Vec3{T}}
    mass::T
end

function CelestialBody(N::Number)
    pos = [Vec3(0.0) for i in 1:N]
    vel = [Vec3(0.0) for i in 1:N]
    CelestialBody(pos, vel, 0.0)
end

function init!(body::CelestialBody{T}, pos::Vec3{T}, vel::Vec3{T}, mass::T) where {T}
    body.pos[1] = pos
    body.vel[1] = vel
    body.mass = mass
end

init(body, pos::T,         vel::T, mass)           where {T<:Real} = init(body, Vec3(pos), Vec3(vel), mass)
init(body, pos::Array{T},  vel::Array{T, 2}, mass) where {T<:Real} = init(body, Vec3(pos), Vec3(vel), mass)
init(body, pos::Array{T},  vel::Vec3{T}, mass)     where {T<:Real} = init(body, Vec3(pos), vel, mass)
init(body, pos::T,         vel::Vec3{T}, mass)     where {T<:Real} = init(body, Vec3(pos), vel, mass)
init(body, pos::Vec3{T},   vel::Array{T, 2}, mass) where {T<:Real} = init(body, pos,       Vec3(vel), mass)
init(body, pos::Vec3{T},   vel::T, mass)           where {T<:Real} = init(body, pos,       Vec3(vel), mass)

function distance(self::CelestialBody, other::CelestialBody, n)
    norm(other.pos[n] - self.pos[n])
end

function tomatrix(body::CelestialBody)
    output = zeros(length(body.pos), 3)
    for n in 1:length(body.pos)
        output[n, :] = tomatrix(body.pos[n])
    end
    output
end


pprint(x) = show(IOContext(STDOUT, limit=false), "text/plain", x)
show(body::CelestialBody) = pprint(body.mass)

end
