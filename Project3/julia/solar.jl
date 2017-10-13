include("odesolver.jl")
using ODESolver

N = 1000
x = zeros(N,2)
y = zeros(N)
vx = zeros(x)
vy = zeros(y)
M⊙ = 2e30
ME = 6e26
rE = 1.5e11
GM⊙ = 4π^2 # [Au³/y²]
r(x) = norm(x)
a(x) = -4π^2x/r(x)^3

