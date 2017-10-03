include("schroedinger.jl")
include("jacobi.jl")
using Schroedinger
using Jacobi
using PyCall
using LaTeXStrings
using PyPlot

@pyimport seaborn as sns
sns.set(style="white", context="poster")
PyPlot.matplotlib[:rc]("font", family="serif")

function plotwavefunction()
    fig, ax = subplots(1)
    ρmin, ρmax, N = 1e-4, 10, 500
    H, R = solve(ρmin, ρmax, N, method=:julia)
    ρ = linspace(ρmin, ρmax, N+1)
    M = 5
    Λ = diag(H)[1:M]
    for m in 1:M
        Ψ = Λ[m]+18*R[:, m]
        ax[:fill_between](ρ, Λ[m], Ψ, alpha=0.5)
        ax[:plot](ρ, Ψ)
        ax[:text](ρ[end], Λ[m],latexstring("\$ \\Psi_{$m}(\\rho)\$"))
    end
    p = ρ[2:end]
    ax2 = ax[:twinx]()
    ax2[:plot](p, p.^2, "k", label=L"\rho^2")
    ax2[:plot](p, p.^2+1./p, "k--", label=L"\rho^2\omega_r + 1/\rho")
    ax[:set_xlabel](L"\rho")
    ax[:set_ylabel](L"Energy")
    ax2[:legend](loc="upper center", bbox_to_anchor=(0.5, 1.05), ncol=2)
    sns.despine()
    ax[:tick_params](axis="y", which="both", bottom="off", top="off", labelleft="off")
    ax2[:tick_params](axis="y", labelleft="off")
    fig[:savefig]("../latex/figures/wavefunctions.png", bbox_inches=:tight)
    show()
end

plotwavefunction()
