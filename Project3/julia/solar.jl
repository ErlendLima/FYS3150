include("odesolver.jl")
using ODESolver


pprint(x) = show(IOContext(STDOUT, limit=true), "text/plain", x)
N = 10
x = zeros(N+1, 2)
y = zeros(N+1, 2)
vx = zeros(x)
vy = zeros(y)
# M⊙ = 2e30
ME = 6e26
rE = 1.5e11
# GM⊙ = 4π^2 # [Au³/y²]
# r(x) = norm(x)
# a(x) = -4π^2x/r(x)^3
h = 1.0

x[1, :] = [0.0 1.0]
v[1, :] = [0.0 0.5]

for i in 1:N
    r = norm(x[i, :])
    a = -4π^2*x[i, :]/r^3
    v[i+1, :] = v[i, :] + a*h
    x[i+1, :] = x[i, :] + v[i, :]*h
end

output = open("../data/test", "w")
write(output, x)
pprint(x)
