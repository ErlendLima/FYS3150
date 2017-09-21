include("jacobi.jl")
module Schroedinger
using Jacobi
export solve, hamiltonian

function hamiltonian(ρmin, ρmax, N)
    h = (ρmax - ρmin)/N
    ρ = ρmin:h:ρmax
    V = ρ.^2
    d = 2/h^2 + V
    e = fill(-1/h^2, N)
    H = SymTridiagonal(d, e)
end

function solve(ρmin, ρmax, N)
    H = full(hamiltonian(ρmin, ρmax, N))
    R = jacobi!(H)
    H, R
end

end
