using PyPlot
using Formatting
using PyCall

@pyimport seaborn as sns
sns.set()
# Code for project1. Is as fast as C++ Armadillo, except when
# garbage collection kicks in
# Loading Plot-package takes a long time since all dependencies
# needs to be compiled at each startup

function solve_special(N)
    for n in N
        h = 1/(n+1)
        printfmtln("Solving $n×$n with stepsize {:.03e}", h)

    end
end

function solve_general(N)
    lower_bound = 0.0
    upper_bound = 0.0
    for n in N
        h = 1/(n+1)
        printfmtln("Solving $n×$n with stepsize {:.03e}", h)
        x::Array{Float64, 1} = collect(0:h:1)
        f = 100exp.(-10x)h^2

        a = fill(-1.0, n+2)
        b = fill(2.0, n+2)
        c = fill(-1.0, n+2)

        # Set the boundary conditions
        a[1]   = 0;   b[1] = 1; c[1]   = 0
        a[end] = 0; b[end] = 1; c[end] = 0
        f[1] = lower_bound
        f[end] = upper_bound

        @time solution = thomas(a, b, c, f)
        plot(x, solution, label="n = $n")
    end
    x = linspace(0,1,1000)
    y = 1 - (1-exp(-10))x - exp.(-10x)
    plot(x, y, label="analytic")
    legend()
    show()
end

function thomas(a, b, c, v)
    # This works properly
    n = length(a)
    c′ = zeros(n)
    v′ = zeros(n)
    u = zeros(n)

    b′ = b[1]
    c′[1] = c[1]/b′
    v′[1] = v[1]/b′

    for i in 2:n
        b′    = b[i] - a[i]c′[i-1]
        v′[i] = (v[i] - a[i]v′[i-1])/b′
        c′[i] = c[i]/b′
    end

    u[n] = v′[n]
    for i in n-1:-1:1
        u[i] = v′[i] - c′[i]u[i+1]
    end
    return u
end

function thomast(a, b, c, f)
    n  = length(a)
    b′ = zeros(n)
    v′ = zeros(n)
    u  = zeros(n)
    c′ = zeros(n)

    b′[1] = b[1]

    for i in 2:n
        c′[i] = c[i-1]/b′[i-1]
        b′[i] = b[i] - a[i]/b′[i-1]*c[i-1]
        v′[i] = (f[i] - a[i]/b′[i-1]*f[i-1])/b′[i]
    end

    u[n] = v′[n]/b′[n]
    for i in n-1:-1:2
        u[i] = v′[i] - c′[i]*u[i+1]
    end
    return u

end

function Thomas(a, b, c, f)
    n   = length(a)
    tmp = zeros(n)
    u   = zeros(n)

    # Forward Sweep
    btmp = b[2]
    u[2] = f[2]/btmp
    for i in 3:n-1
        tmp[i] = c[i-1]/btmp
        btmp   = b[i]-a[i]*tmp[i]
        u[i]   = (f[i]-a[i]*u[i-1])/btmp
    end
    # Backwards Sweep
    for i in n-1:-1:2
        u[i] -= tmp[i+1]*u[i+1]
    end

    return u
end


solve_general([10^n for n in 1:5])
