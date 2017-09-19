include("schroedinger.jl")
using Schroedinger

N = 5
H, R = solve(0, 1, N)
d = sort(diag(H))
e = sort(eigvals(hamiltonian(0, 1, N)))
for i in 1:N
    println("$(d[i]) ≈ $(e[i])")
end

