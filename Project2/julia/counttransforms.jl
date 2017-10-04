include("schroedinger.jl")
include("jacobi.jl")
using Schroedinger
using Jacobi
using PyCall
using LaTeXStrings
using PyPlot

@pyimport seaborn as sns
sns.set(style="white", context="poster")

function counttransforms()
    #= Plots the number of iterations necessary to make
       all off diagonal elements less than ε = 10⁻⁸
    =#
    iterations = []
    jacobi!(full(hamiltonian(0, 1, 5))) # compile it
    Ns = 5:5:200
    for N in Ns
        ρmin, ρmax = 1e-40, 50
        H = hamiltonian(ρmin, ρmax, N)
        H = full(H)
        H, iteration = jacobi!(H)
        append!(iterations, iteration)
    end
    fig, ax = subplots(1)
    ax[:plot](Ns, iterations, label="Jacobi")
    ax[:plot](Ns, 0.0021875*Ns.^3, label=L"0.0021875N^3")
    ax[:set_xlabel]("Mesh size")
    ax[:set_ylabel]("Number of iterations")
    ax[:legend]()
    sns.despine()
    fig[:savefig]("../latex/figures/iterations.eps", bbox_inches=:tight)
    show()
end

counttransforms()

