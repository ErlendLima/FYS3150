include("vector3d.jl")
include("celestial.jl")
include("solver.jl")
import Vector3D: Vec3
import Celestial: CelestialBody, init!
import SystemSolver: Solver, addbody!, init!, solve!
pprint(x) = show(IOContext(STDOUT, limit=false), "text/plain", x)

function run()
    N = 30000
    h = 1/10000
    earth = CelestialBody(N+1)
    sun   = CelestialBody(N+1)
    init!(earth, Vec3(1.0, 0.0, 0.0), Vec3(0.0, 2π, 0.0), 3e-6)
    init!(sun, Vec3(0.0), Vec3(0.0), 1.0)

    solver = Solver()
    addbody!(solver, [sun earth])
    solve!(solver, N, h, :euler)

    # Do some black magic to convert the matrix to the correct size
    output = tomatrix.(bodies) |> x->hcat(x...)
    writedlm("../data/julia.txt", output)
end

run()
