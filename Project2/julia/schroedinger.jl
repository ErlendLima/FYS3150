include("jacobi.jl")
module Schroedinger
using Jacobi
export solve, hamiltonian

function hamiltonian(ρmin, ρmax, N; lOrbital=0, potential=:plain, μ=1, ωᵣ=1)
    #= Creates a matrix describing the scaled Hamiltonian
       of an electron in a harmonic oscillator
       @Input: ρmin, the lower limit of ρ
               ρmax, the upper limit of ρ
               N, the mesh size
               lOrbital, the orbital angular momentum of the electron
               potential, which potential to use. Can be :plain, :exponential or :repulsive
               μ, ummm
               ωᵣ, parameter which reflects the strength of the oscillator potential
       @Returns: A tridiagonal symmetric matrix describing the Hamiltonian
    =#
    h = (ρmax - ρmin)/N
    ρ = ρmin:h:ρmax

    # Compute the potential of the system
    orbitalFactor = lOrbital⋅(lOrbital + 1.0)
    if potential == :plain
        # println("Using plain potential")
        potential = ρ.^2
    elseif potential == :exponential
        # println("Using exponential potential")
        potential = ρ.^2 + exp(-μ*ρ)./ρ.^2
    elseif potential == :repulsive
        # println("Using repulsive potet")
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
    #= Finds the eigenvalues and eigenvectors of the Hamiltonian
       @Input: ρmin, the lower limit of ρ
               ρmax, the upper limit of ρ
               N, the mesh size
               method, the algorithm to utilize. Can be :jacobi or :julia
               lOrbital, the orbital angular momentum of the electron
               potential, which potential to use. Can be :plain, :exponential or :repulsive
               μ, ummm
               ωᵣ, parameter which reflects the strength of the oscillator potential
       @Returns: The eigenvalues and eigenvectors of the Hamiltonian
    =#
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
