include("jacobi.jl")
module Schroedinger
using Jacobi
export solve, hamiltonian

function hamiltonian(ρmin, ρmax, N; lOrbital=0, potential=:r², mu=1)
    h = (ρmax - ρmin)/N
    ρ = ρmin:h:ρmax

    # Compute the potential of the system
    orbitalFactor = lOrbital⋅(lOrbital + 1.0)
    if potential == :r²
        potential = ρ.^2
    elseif potential == :repulsion
        potential = ρ.^2 + exp(-mu*ρ)./ρ.^2
    end
    V = potential + orbitalFactor./ρ.^2

    # Set up the matrix
    diagonal    = 2/h^2 + V
    offdiagonal = fill(-1/h^2, N)
    H = SymTridiagonal(diagonal, offdiagonal)
end

function solve(ρmin, ρmax, N; method=:jacobi, potential=:repulsion)
    H = hamiltonian(ρmin, ρmax, N, potential=potential)
    if method == :jacobi
        H = full(H)
        R = jacobi!(H)
    elseif method == :julia
        R = eigvecs(H)
        eigvals!(H)
    end
    H, R
end

end
