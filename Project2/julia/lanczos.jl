module Lanczos
export lanczos

function lanczos(A:<Tridiagonal)
    m = size(T, 1)
    q = zeros(T)
    r = zeros(T)
    β = ones(T)
    α = zeros(T)
    I = eye(T)
    k = 1
    q[1] = r[1]/β[1] # This makes no sense
    while β[k] ≠ 0
        q[k+1] = r/β[k]
        k += 1
        α[k] = q[k]'*A*q[k]
        r[k] = (A-q[k]*I)*q[k] - β[k-1]*q[k-1]
        β[k] = vecnorm(A)
    end

end
end
