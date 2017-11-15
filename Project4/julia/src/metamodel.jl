module Metamodel
export Metadata, writedata!

using JSON

mutable struct Metadata
    latticesize::T           where T<:Integer
    MCiterations::T          where T<:Integer
    temperature::T           where T<:Real
    saveperiod::T            where T<:Integer
    J::T                     where T<:Integer
    h::T                     where T<:Real
    initialorientation::Symbol
    basepath::String
    energypath::String
    magneticmomentpath::String
    evolutionpath::String
    metadatapath::String
    seed::T                  where T<:Real
    rng::MersenneTwister

    energydim::Tuple
    magneticmomentdim::Tuple
    evolutiondim::Tuple

    function Metadata{I<:Integer, R<:Real}(size::I, iter::I, temp::R, orientation::Symbol,
                      path::String; seed=rand(Int))
        # Construct a new model by checking that each argument is valid
        # and using default values for several variables

        size ≤ 0 && throw(ArgumentError("The lattice size must be positive."))
        iter ≤ 0 && throw(ArgumentError("The number of MC iterations must be positive."))
        temp ≤ 0 && throw(ArgumentError("The temperature [K] must be positive."))
        seed ≤ 0 && throw(ArgumentError("The random seed must be positive"))
        orientation ∉ [:up, :down, :random] && throw(ArgumentError("The orientation must be :up, :down or :random"))

        # All IO paths
        if !endswith(path, "/")
            path = path * "/"
        end
        energypath = "energies.bin"
        evolutpath = "evolution.bin"
        momentpath = "magneticmoment.bin"
        metadatapath = "meta.json"

        # Construct the object
        new(size, iter, temp, 1000, 1, 0, orientation,
            path, energypath, momentpath, evolutpath, metadatapath,
            seed, MersenneTwister(seed), (0,), (0,), (0,0))
    end
end

function savedimensions!(model::Metadata, evolution, energy, magneticmoment)
    model.energydim = size(energy)
    model.magneticmomentdim = size(magneticmoment)
    model.evolutiondim = size(evolution)
end

function writedata!(model::Metadata, evolution::Array{Int8, 3}, energy::Array{T}, magneticmoment::Array{T}) where T<:Real
    output = [(model.energypath, energy), (model.magneticmomentpath, magneticmoment)]
    for (path, data) in output
        open(x -> write(x, data), model.basepath * path, "w")
    end

    stream = open(model.basepath * model.evolutionpath, "w")
    for m in 1:size(evolution, 1)
        write(stream, evolution[m, :, :])
    end

    savedimensions!(model, evolution, energy, magneticmoment)
    writemetadata(model)
end

function writedata!(model::Metadata, evolution::Array{Int8, 2}, energy::T, magneticmoment::T) where T<:Real
    output = [(model.energypath, energy),
              (model.magneticmomentpath, magneticmoment),
              (model.evolutionpath, evolution)]
    for (path, data) in output
        stream = open(model.basepath * path, "w")
        write(stream, data)
    end
    savedimensions!(model, evolution, energy, magneticmoment)
    writemetadata(model)
end

function writemetadata(model::Metadata)
    stream = open(model.basepath * model.metadatapath, "w")
    magneticmeta = Dict("path" => model.magneticmomentpath,
                        "dim"  => model.magneticmomentdim,
                        "type" => "float64")
    energymeta = Dict("path" => model.energypath,
                      "dim"  => model.energydim,
                      "type" => "float64")
    evolutionmeta = Dict("path" => model.evolutionpath,
                         "dim"  => model.evolutiondim,
                         "type" => "int8")
    metadata = Dict("energy"          => energymeta,
                    "magnetic moment" => magneticmeta,
                    "evolution"       => evolutionmeta,
                    "saveperiod"      => model.saveperiod,
                    "seed"            => model.seed,
                    "lattice size"    => model.latticesize,
                    "MC iterations"   => model.MCiterations)
    JSON.print(stream, metadata)
end

end
