using PyPlot

# Code for project1. Is as fast as C++ Armadillo, except when
# garbage collection kicks in

function solveSpecial(N::Array{Int64})
    for n in N
        h = 1/(n-1)
    end
end

function solveGeneral(N::Array{Int64})
    for n in N
        h = 1/(n-1)
        println("Solving $n×$n with stepsize $h")
        x::Array{Float64, 1} = collect(0:h:1)
        f = 100exp.(-10x) * h^2

        a = fill(-1.0, n)
        b = fill(2.0, n)
        c = fill(-1.0, n)


        @time solution = thomast(a, b, c, f)
        plot(x, solution, label="n = $n")
    end
    x = linspace(0,1,1000)
    y = 1 - (1-exp(-10))x - exp.(-10x)
    plot(x, y, label="analytic")
    legend()
    show()
end

function thomas(a::Array{Float64, 1}, b::Array{Float64, 1},
                c::Array{Float64, 1}, f::Array{Float64, 1})
    n::Int32 = length(a)
    c′::Array{Float64,1} = zeros(n)
    d′::Array{Float64,1} = zeros(n)
    u::Array{Float64,1} = zeros(n)

    # Forward Sweep
    c′[1] = c[1]/b[1]
    d′[1] = f[1]/b[1]
    for i in 2:n
        c′[i] = c[i]/(b[i] - a[i]*c′[i-1])
        d′[i] = (f[i]-a[i]*d′[i-1])/(b[i]-c′[i-1])
    end
    # Backwards Sweep
    u[n] = d′[n]
    for i in n-1:-1:1
        u[i] = d′[i] - c′[i]*u[i+1]
    end
    return u
end

function thomast(a::Array{Float64, 1}, b::Array{Float64, 1},
                c::Array{Float64, 1}, f::Array{Float64, 1})
    n::Int32 = length(a)
    tmp::Array{Float64,1} = zeros(n)
    u::Array{Float64,1} = zeros(n)

    # Forward Sweep
    btmp::Float64 = b[1]
    u[1]          = f[1]/btmp
    for i in 2:n
        tmp[i] = c[i-1]/btmp
        btmp   = b[i]-a[i]*tmp[i]
        u[i]   = (f[i]-a[i]*u[i-1])/btmp
    end

    # Backwards Sweep
    u[end] = 0
    for i in n-1:-1:1
        u[i] -= tmp[i+1]*u[i+1]
    end

    return u
end


solve([10^n for n in 1:5])
