include("odesolver.jl")
include("vector3d.jl")
include("celestial.jl")
using ODESolver
importall Vector3D
importall Celestial
pprint(x) = show(IOContext(STDOUT, limit=false), "text/plain", x)

N = 1000
h = 0.01
earth = CelestialBody(N+1)
sun = CelestialBody(N+1)
# jupiter = CelestialBody(N+1)
init(earth, Vec3(1.0, 0.0, 0.0), Vec3(0.0, 2π, 0.0), 3e-6)
init(sun, Vec3(0.0), Vec3(0.0), 1.0)
# init(jupiter, [70.0 1.0 1.0], Vec3(0.5), 3.0)

bodies = [sun earth]
G = 4π^2
for n in 1:N
    # Calculate forces
    for body in bodies
        for other in bodies
            body ≡ other && continue
            r = distance(body, other, n)
            a = -G*other.mass*body.pos[n]/r^3
            body.vel[n+1] = body.vel[n] + a*h
            body.pos[n+1] = body.pos[n] + body.vel[n]*h
        end
    end
end

pprint(earth.pos[1:10])
pprint(sun.pos[1:10])

# Do some black magic to convert the matrix to the correct size
output = tomatrix.(bodies) |> x->hcat(x...)
writedlm("../data/julia.txt", output)
