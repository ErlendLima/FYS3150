module Ising
export neighbours, ising!, hamiltonian, Metadata

using  MPI
include("./metamodel.jl")
using .Metamodel: Metadata, writedata!

pprint(x) = show(IOContext(STDOUT, limit=false), "text/plain", x)
function pprintln(x)
    pprint(x)
    println("")
end

function mprint(x)
    for i in 1:size(x, 1)
        for j in 1:size(x, 2)
            if x[i, j] == 1
                print("██")
            else
                print("▒▒")
            end
        end
        print("\n")
    end
end


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


function constructlattice(model)
    # Use Int8 to use as little memory as possible
    if model.initialorientation == :random
        # Construct the lattice with randomly oriented spins.
        rand(model.rng, convert(Vector{Int8}, [-1, 1]),
             (model.latticesize, model.latticesize))
    elseif model.initialorientation == :up
        ones(Int8, (model.latticesize, model.latticesize))
    elseif model.initialorientation == :down
        -ones(Int8, (model.latticesize, model.latticesize))
    else
        throw(ArgumentError("Initial orientation $(model.initialorientation) is not valid."))
    end
end

function initializesystem(model::Metadata)
    N = model.latticesize
    M = model.MCiterations
    J = model.J
    h = model.h
    temperature = model.temperature
    saveperiod = model.saveperiod
    β = 1.0/(1*temperature)

    Λ = constructlattice(model)

    # Since the state space and hence energy space is discrete,
    # the probabilities can be precalculated
    # This is the entire state space to ΔE
    ΔEstates = [-4J, -2J, 0, 2J, 4J]
    # And the probability for each ΔE
    ratio = Dict(i => exp(-β*i) for i in ΔEstates)

    # Matrices to store data for analysis
    if model.version == :parallel
        evolution = zeros(Int8, (M, N, N))
        energies = zeros(Float64, (M-1)*N^2+1)
        magneticmoment = zeros(Float64, (M-1)*N^2+1)
    elseif model.version == :serial
        evolution = zeros(Int8, (N, N))
        energy = 0.0
        magneticmoment = 0.0
    else
        throw(ArgumentError("Unknown model: $(model.version)"))
    end
    (lattice, ratios, evolution, energies, magneticmoment)
end


function ising!(model::Metadata, version::Symbol)
    if version == :parallel
        isingparallel!(model)
    elseif version == :serial
        isingserial!(model)
    else
        throw(ArgumentError("$version is not supported"))
    end
end


function isingserial!(model::Metadata)
    N = model.latticesize
    M = model.MCiterations
    Λ, ratio, evolution, energies, magneticmoment = initializesystem(model)

    # Initial values
    evolution[1, :, :] = Λ
    energies[1] = hamiltonian(Λ, J)
    magneticmoment[1] = sum(Λ)

    # Metropolis Algorithm
    @time for m in 2:M
        counter = 1
        for index in eachindex(Λ)
            n = (N*N)*(m-2) + counter + 1
            i, j = rand(model.rng, 1:N, (1, 2))
            ΔE = J*Λ[i, j]*sum(neighbours(i, j, Λ))
            if ΔE < 0 || rand(model.rng) <= ratio[ΔE]
                Λ[i, j] = -Λ[i, j]
                energies[n] = energies[n-1] + ΔE
                magneticmoment[n] = magneticmoment[n-1] + 2*Λ[i, j]
            else
                energies[n] = energies[n-1]
                magneticmoment[n] = magneticmoment[n-1]
            end
            counter = counter + 1
        end
        evolution[m, :, :] = Λ
    end
    writedata!(model, evolution, energies, magneticmoment)
end

function isingparallel!(model::Metadata)
    N = model.latticesize
    M = model.MCiterations
    Λ, ratio, evolution, energies, magneticmoment = initializesystem(model)

    # Initial values
    energies = hamiltonian(Λ, J)
    magneticmoment = sum(Λ)

    # Metropolis Algorithm
    @time for m in 2:M
        for index in eachindex(Λ)
            i, j = rand(model.rng, 1:N, (1, 2))
            ΔE = J*Λ[i, j]*sum(neighbours(i, j, Λ))
            if ΔE < 0 || rand(model.rng) <= ratio[ΔE]
                Λ[i, j] = -Λ[i, j]
                energy += ΔE
                magneticmoment += 2*Λ[i, j]
            end
        end
    end
    println("Energy $energy")
    println("Magnetic moment $magneticmoment")
    mprint(Λ)
    # writedata!(model, Λ, energy, magneticmoment)
end

end
