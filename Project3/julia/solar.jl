include("odesolver.jl")
include("vector3d.jl")
include("celestial.jl")
using ODESolver
importall Vector3D
importall Celestial
pprint(x) = show(IOContext(STDOUT, limit=false), "text/plain", x)

N = 5000
h = 1e-3
earth = CelestialBody(N+1)
sun = CelestialBody(N+1)
jupiter = CelestialBody(N+1)
init(earth, Vec3(1.1, 2.0, 3.0), Vec3(2.0), 1.0)
init(sun, Vec3(0.0), Vec3(0.0), 1000.0)
init(jupiter, [70.0 1.0 1.0], Vec3(0.5), 3.0)

bodies = [sun earth jupiter]
for n in 1:N
    for body in bodies
        for other in bodies
            body ≡ other && continue
            r = distance(body, other, n)
            a = -4π^2*body.pos[n]/r^3
            body.vel[n+1] = body.vel[n] + a*h
            body.pos[n+1] = body.pos[n] + body.vel[n]*h
        end
    end
end

# x = zeros(N+1, n)
# y = zeros(N+1, n)
# vx = zeros(x)
# vy = zeros(y)
# # M⊙ = 2e30
# ME = 6e26
# rE = 1.5e11
# # GM⊙ = 4π^2 # [Au³/y²]
# # r(x) = norm(x)
# # a(x) = -4π^2x/r(x)^3
# h = 1.0

# x[1, :] = [0.0 1.0]
# v[1, :] = [0.0 0.5]

# for i in 1:N
#     for j in 1:n
#     r = norm(x[i, :])
#     a = -4π^2*x[i, :]/r^3
#     v[i+1, :] = v[i, :] + a*h
#     x[i+1, :] = x[i, :] + v[i, :]*h
# end


# Do some black magic to convert the matrix to the correct size
output = tomatrix.(bodies) |> x->hcat(x...)
writedlm("../data/julia.txt", output)
