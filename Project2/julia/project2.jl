include("schroedinger.jl")
using Schroedinger
using PyCall
using LaTeXStrings
using PyPlot

@pyimport seaborn as sns
sns.set(style="whitegrid")

pprint(x) = show(IOContext(STDOUT, limit=true), "text/plain", x)
N = 500
ρ_min, ρ_max = 1e-4, 10
H, R = solve(1e-4, 10, N, method=:julia, potential=:plain)
H2, R2 = solve(1e-4, 10, N, method=:julia, potential=:repulsive)
# E = diag(H2)
# pprint(E)

x = linspace(ρ_min, ρ_max, N+1)
fig, (ax1, ax2) = subplots(2, sharex=:true)
ax1[:plot](x, R[:, 1].^2, label="Plain potential")
ax1[:plot](x, R2[:, 1].^2, label="Repulsive potential")
ax2[:plot](x, R[:, 2].^2, label="Plain potential")
ax2[:plot](x, R2[:, 2].^2, label="Repulsive potential")
ax1[:set_title]("Ground state")
ax2[:set_title]("First excited state")
ax2[:set_xlabel](L"\rho")
ax1[:set_ylabel](L"|u(\rho)|^2")
ax2[:set_ylabel](L"|u(\rho)|^2")
ax1[:legend]()
ax2[:legend]()
fig[:savefig]("../latex/figures/excitation.eps")


fig, ax = subplots(1)
for ω in [0.01, 0.5, 1, 5]
    H, R = solve(ρ_min, ρ_max, N, method=:julia, potential=:repulsive, ωᵣ=ω)
    ax[:plot](x, R[:, 1].^2, label=latexstring("\$\\omega_r = $ω\$"))
end
H, R = solve(ρ_min, ρ_max, N, method=:julia)
ax[:plot](x, R[:, 1].^2, "--", label="Non-interacting")
ax[:set_title](L"Particles in ground states with different values of $\omega_r$")
ax[:set_xlabel](L"\rho")
ax[:set_ylabel](L"|u(\rho)|^2")
ax[:legend]()
fig[:savefig]("../latex/figures/omegas.eps")
show()
