include("jacobi.jl")
module Schroedinger
using Jacobi
export solve, hamiltonian

function hamiltonian(ρmin, ρmax, N; lOrbital=0, potential=:plain, μ=1, ωᵣ=1)
    h = (ρmax - ρmin)/N
    ρ = ρmin:h:ρmax

    # Compute the potential of the system
    orbitalFactor = lOrbital⋅(lOrbital + 1.0)
    if potential == :plain
        println("Using plain potential")
        potential = ρ.^2
    elseif potential == :exponential
        println("Using exponential potential")
        potential = ρ.^2 + exp(-μ*ρ)./ρ.^2
    elseif potential == :repulsive
        println("Using repulsive potet")
        potential = ρ.^2*ωᵣ^2 + 1./ρ
    else
        throw(ArgumentError("Expected either :plain or :repulsion"))
    end
    V = potential + orbitalFactor./ρ.^2

    # Set up the matrix
    diagonal    = 2/h^2 + V
    offdiagonal = fill(-1/h^2, N)
    H = SymTridiagonal(diagonal, offdiagonal)
end

function solve(ρmin, ρmax, N; method=:jacobi, lOrbital=0, potential=:plain, μ=1, ωᵣ=1)
    H = hamiltonian(ρmin, ρmax, N, lOrbital=lOrbital, potential=potential, μ=μ, ωᵣ=ωᵣ)
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
