module Ising
export neighbours, ising, hamiltonian

pprint(x) = show(IOContext(STDOUT, limit=false), "text/plain", x)
function pprintln(x)
    pprint(x)
    println("")
end

mutable struct model
    Λ::Array{Int8}
    temperature::T where T<:Real
    latticesize::T where T<:Integer
    saveperiod::T  where T<:Integer
    J::T           where T<:Real
    h::T           where T<:Real
end

function init() end

function neighbours(i, j, A)
    N = size(A, 1)
    function period(x)
        # First ensure that the number is not negative
        # then wrap it around, and add 1 since Julia
        # arrays start at 1
        a = (x + N+1) % (N+1)
        if a == 0
            1
        else
            a
        end
    end
    [A[period(i-1), j], A[i, period(j-1)], A[i, period(j+1)], A[period(i+1), j]]
end

function hamiltonian(A, J)
    n, m = size(A)
    E = 0
    for i ∈ 1:n, j ∈ 1:m
        E -= A[i, j]*sum(neighbours(i, j, A))
    end
    return J*E
end

function hamiltonian(A, J, h)
    hamiltonian(A, J) - h*sum(A)
end

function savemodel(output, file)
    stream = open(file, "w")
    for m in 1:size(output, 1)
        write(stream, output[m, :, :])
    end
end

function savesnapshot(m, writeperiod, evolution, energies, magneticmoment)
    if m % saveperiod == 0
        # save
    end
end

function ising()
    N = 50
    M = 1000
    J = 1
    h = 0
    temperature = 1.00
    saveperiod = 100
    β = 1.0/(1*temperature)
    # Define the lattice Λ with randomly oriented spins.
    # Use Int8 to use as little memory as possible
    rng = MersenneTwister(1233)
    Λ = rand(rng, convert(Vector{Int8}, [-1, 1]), (N, N))

    # Matrices to store data for analysis
    evolution = zeros(Int8, (M, N, N))
    energies = zeros(Float64, M*N^2)
    magneticmoment = zeros(Float64, M*N^2)

    # Since the state space and hence energy space is discrete,
    # the probabilities can be precalculated
    # This is the entire state space to ΔE
    ΔEstates = [-4J, -2J, 0, 2J, 4J]
    # And the probability for each ΔE
    ratio = Dict(i => exp(-β*i) for i in ΔEstates)

    # Initial values
    evolution[1, :, :] = Λ
    energies[1] = hamiltonian(Λ, J)
    magneticmoment[1] = sum(Λ)

    # Metropolis Algorithm
    for m in 2:M
        for σᵢⱼ in Λ
            i, j = rand(rng, 1:N, (1, 2))
            ΔE = J*Λ[i, j]*sum(neighbours(i, j, Λ))
            if ΔE < 0 || rand(rng) <= ratio[ΔE]
                Λ[i, j] = -Λ[i, j]
                energies[m] = energies[m-1] + ΔE
                magneticmoment[m] = magneticmoment[m-1] + 2*Λ[i, j]
            else
                energies[m] = energies[m-1]
                magneticmoment[m] = magneticmoment[m-1]
            end
        end
        evolution[m, :, :] = Λ
    end
    savemodel(evolution, "../../data/evolution$(M)by$(N)by$(N).bin")
    energystream = open("../../data/energies$(M).bin", "w")
    write(energystream, energies)
    mmomentstream = open("../../data/magneticmoment$(M).bin", "w")
    write(mmomentstream, magneticmoment)
end

end
