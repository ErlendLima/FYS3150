include("schroedinger.jl")
include("jacobi.jl")
using Schroedinger
using Jacobi
using PyCall
using PyPlot

@pyimport seaborn as sns
sns.set(style="white", context="poster")

function pprint(x)
    show(IOContext(STDOUT, limit=true), "text/plain", x)
    println(" ")
end

function timejacobi()
    iterations = []
    jacobi!(full(hamiltonian(0, 1, 5))) # compile it
    Ns = 5:5:200
    for N in Ns
        ρmin, ρmax = 1e-40, 50
        H = hamiltonian(ρmin, ρmax, N)
        H = full(H)
        H, iteration = jacobi!(H, getiterations=true)
        append!(iterations, iteration)
    end
    fig, ax = subplots(1)
    ax[:plot](Ns, iterations)
    ax[:set_xlabel]("Mesh size")
    ax[:set_ylabel]("Number of iterations")
    ax[:legend]()
    sns.despine()
    fig[:savefig]("../latex/figures/iterations.eps", bbox_inches=:tight)
    show()
end

timejacobi()

