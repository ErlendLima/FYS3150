module ODESolver
export forwardeuler, velocityverlet

function forwardeuler!(x, y, N, h, a::Function)
    for i in 1:N
        v[i+1] = v[i] + h*a(x)
        x[i+1] = x[i] + h*v[i]
    end
end
end
