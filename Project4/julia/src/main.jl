include("ising.jl")
 # include("metamodel.jl")
using .Ising
# using .Metamodel

function run()
    model = Metadata(20, 1000, 1.0, :random, 1234, "../../data/")
    ising!(model, :parallel)
end

run()

