include("schroedinger.jl")
include("jacobi.jl")
using Schroedinger
using Jacobi
using PyCall
using PyPlot

@pyimport seaborn as sns
sns.set(style="white", context="poster")

function loadcpptiming()
    #= Reads the files containing the timings from the cpp program
       when using the custom jacobi or armadillo's method for finding
       eigenvalues.
       @Returns: two arrays containing the timings of type array{Float64}
    =#
    arma = parse.(Float64, open(readlines, "../cpp/data/timings_arma.txt", "r"))
    jacobi = parse.(Float64, open(readlines, "../cpp/data/timings_jacobi.txt", "r"))
    arma, jacobi
end


function timejacobi()
    #= Finds the eigenvalues of n×n matrices for different n, recording
       the time taken and the memory used by the custom jacobi method and
       julia's method.
       @Returns: five arrays, the first being all of the matrix sizes used
    =#
    jacobitimes, juliatimes = [], []
    jacobimemory, juliamemory = [], []
    jacobi!(full(hamiltonian(0, 1, 5))) # compile it
    Ns = 5:5:200
    for N in Ns
        ρmin, ρmax = 1e-40, 50
        H = hamiltonian(ρmin, ρmax, N)
        H = full(H)
        _, jacobitime, jacobimem, = @timed H, iterations = jacobi!(H)
        _, juliatime, juliamem = @timed H = hamiltonian(ρmin, ρmax, N)
        append!(jacobitimes, jacobitime)
        append!(juliatimes, juliatime)
        append!(juliamemory, juliamem)
        append!(jacobimemory, jacobimem)
    end
    Ns, juliatimes, jacobitimes, juliamemory, jacobimemory
end


function plottimings()
    #= Plots the time used and memory taken by the jacobi methods written
       in julia and c++, comparing them to the time taken by julia's standard
       method and armadillo
    =#
    N, juliatime, jacobitime, juliamemory, jacobimemory = timejacobi()
    cpparma, cppjacobi = loadcpptiming()
    fig, (ax1, ax2) = subplots(2, sharex=:true)
    l1, = ax1[:plot](N, jacobitime)
    l2, = ax1[:plot](N, juliatime, "--")
    l3, = ax1[:plot](N, cppjacobi)
    l4, = ax1[:plot](N, cpparma, "--")
    ax1[:set_ylabel]("Time [s]")
    ax2[:plot](N, jacobimemory./1e3, label="Jacobi")
    ax2[:plot](N, juliamemory./1e3, "--", label="Julia")
    ax2[:set_xlabel]("Mesh size")
    ax2[:set_ylabel]("Allocated memory [KiB]")
    fig[:legend]((l1, l2, l3, l4), ("Jacobi Julia", "Fast Julia",
                                    "Jacobi C++", "Fast Armadillo"),
                 loc="upper center")
    sns.despine()
    fig[:savefig]("../latex/figures/juliatime.eps", bbox_inches=:tight)
    show()
end

plottimings()

