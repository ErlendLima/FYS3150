include("schroedinger.jl")
using Schroedinger


function compare()
    for ω in [1e-5, 1e-3, 1e-2, 0.5, 1, 2, 3, 4]
        H, R = solve(1e-4, 40, 700, method=:julia, potential=:repulsive, ωᵣ=ω)
        m = [0,1,2,3]
        ωₑ = sqrt(3) * ω
        V₀ = 2*3/2*(ω)^(2/3)
        λ = V₀+ωₑ*(m+1/2)
        println(diag(H)[1:4],"\t", λ)
    end
end

compare()
