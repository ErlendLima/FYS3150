include("schroedinger.jl")
using Schroedinger
using PyCall
using LaTeXStrings
import PyPlot
const plt = PyPlot

@pyimport seaborn as sns
sns.set()

N = 500
H, R = solve(0, 10, N, method=:julia)
E = diag(H)

K = 3
x = 0:K/N:K
for k in 1:K
    plt.plot(x, R[:, k].^2, label=latexstring("\$E_{$k} = $(E[k])\$"))
end
plt.plt[:xlabel](L"\rho")
plt.plt[:ylabel](L"|u(\rho)|^2")
plt.plt[:legend]()
plt.show()
