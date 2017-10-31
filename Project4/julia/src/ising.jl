module Ising
export neighbours, ising, hamiltonian

pprint(x) = show(IOContext(STDOUT, limit=false), "text/plain", x)
function pprintln(x)
    pprint(x)
    println("")
end

function neighbours(i, j, A)
    N = size(A)[1]
    function period(x)
        a = ((x+N+1)%(N+1))
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

function ising()
    N = 20
    M = 50000
    J = 1
    h = 0
    temperature = 10.0
    β = 1.0/(1*temperature)
    # Define the lattice Λ with randomly oriented spins.
    # Use Int8 to use as little memory as possible
    rng = MersenneTwister(1233)
    Λ = rand(rng, convert(Vector{Int8}, [-1, 1]), (N, N))

    # Matrices to store data for analysis
    evolution = zeros(Int8, (M, N, N))
    energies = zeros(Float64, M)
    magneticmoment = zeros(Float64, M)


    # Since the state space and hence energy space is discrete,
    # the probabilities can be precalculated
    # This is the entire state space to ΔE
    ΔEstates = [-4J, -2J, 0, 2J, 4J]
    # The partition function
    Z = sum(exp.(-β*ΔEstates))
    # And the probability for each ΔE
    probability = Dict(i => exp(-β*i)/Z for i in ΔEstates)

    # Initial values
    evolution[1, :, :] = Λ
    energies[1] = hamiltonian(Λ, J)
    magneticmoment[1] = sum(Λ)

    # Metropolis Algorithm
    for m in 2:M
        i, j = rand(rng, 1:N, (1, 2))
        ΔE = -J*Λ[i, j]*sum(neighbours(i, j, Λ))
        if ΔE < 0 || rand(rng) > probability[ΔE] # Precompute the probabilities?
            Λ[i, j] = -Λ[i, j]
        end
        evolution[m, :, :] = Λ
        energies[m] = energies[m-1] + ΔE
        magneticmoment[m] = magneticmoment[m-1] + 2*Λ[i, j]
    end
    savemodel(evolution, "../../data/evolution$(M)by$(N)by$(N).bin")
    energystream = open("../../data/energies$(M).bin", "w")
    write(energystream, energies)
    mmomentstream = open("../../data/magneticmoment$(M).bin", "w")
    write(mmomentstream, magneticmoment)
end

end
