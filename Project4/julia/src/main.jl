include("ising.jl")
 # include("metamodel.jl")
using .Ising
# using .Metamodel

function run()
    model = Metadata(50, 1000, 1.0, :random, "../../data/"; seed=1334)
    # model.J = 1
    ising!(model, :serial)
end

run()

