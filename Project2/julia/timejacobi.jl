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
    jacobitimes, juliatimes = [], []
    jacobimemory, juliamemory = [], []
    jacobi!(full(hamiltonian(0, 1, 5))) # compile it
    Ns = 5:5:200
    for N in Ns
        ρmin, ρmax = 1e-40, 50
        H = hamiltonian(ρmin, ρmax, N)
        H = full(H)
        _, jacobitime, jacobimem, = @timed H, iterations = jacobi!(H, getiterations=true)
        _, juliatime, juliamem = @timed H = hamiltonian(ρmin, ρmax, N)
        append!(jacobitimes, jacobitime)
        append!(juliatimes, juliatime)
        append!(juliamemory, juliamem)
        append!(jacobimemory, jacobimem)
    end
    fig, (ax1, ax2) = subplots(2, sharex=:true)
    l1, = ax1[:plot](Ns, jacobitimes, label="Jacobi")
    l2, = ax1[:plot](Ns, juliatimes, label="Julia")
    ax1[:set_ylabel]("Time [s]")
    ax2[:plot](Ns, jacobimemory./1e3, label="Jacobi")
    ax2[:plot](Ns, juliamemory./1e3, label="Julia")
    ax2[:set_xlabel]("Mesh size")
    ax2[:set_ylabel]("Allocated memory [KiB]")
    fig[:legend]((l1, l2), ("Jacobi", "Julia"), loc="upper center")
    sns.despine()
    fig[:savefig]("../latex/figures/juliatime.eps", bbox_inches=:tight)
    show()
end

timejacobi()

