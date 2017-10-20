module SystemSolver
export Solver, forwardeuler, addbody!, init!, solve!

include("vector3d.jl")
include("celestial.jl")
import Base:show, +, -, *
import Vector3D: Vec3, +, -, *
import Celestial: CelestialBody, init!, distance, tomatrix, show

pprint(x) = show(IOContext(STDOUT, limit=false), "text/plain", x)

mutable struct Solver
    N::Integer
    Δt::Float64
    method::Symbol
    bodies::Vector{CelestialBody{Float64}}
    Solver() = new(0, 0.0, :none, Array{CelestialBody{Float64}}(2))
end

function addbody!(solver::Solver, body::CelestialBody)
    @show solver.bodies
    append!(solver.bodies, body)
end

function addbody!(solver::Solver, bodies::Array{CelestialBody{T}}) where {T}
    for body in bodies
        addbody!(solver, body)
    end
end

function init!(solver::Solver, N::Integer, Δt::Float64)
    solver.N = N
    solver.Δt = Δt
end

function solve!(solver::Solver, N::Integer, Δt::Float64, method::Symbol)
    init!(solver, N, Δt)
    if method == :euler
        forwardeuler!(solver)
    else
        throw("$method not recognized")
    end

    save(solver)
end

⟹(x,y) = x && y
function forwardeuler!(solver::Solver)
    G = 4π^2
    @time for n in 1:solver.N
        for body ∈ solver.bodies, other ∈ solver.bodies
            body ≡ other ⟹ continue
            r = distance(body, other, n)
            a = -G*other.mass*body.pos[n]/r^3
            body.vel[n+1] = body.vel[n] + a*h
            body.pos[n+1] = body.pos[n] + body.vel[n]*h
        end
    end
end

function save(solver::Solver)
    # Do some black magic to convert the matrix to the correct size
    output = tomatrix.(solver.bodies) |> x->hcat(x...)
    writedlm("../data/julia.txt", output)
end
end
