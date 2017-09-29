module Jacobi
export offdiagmax, jacobirotate!, jacobi!

function jacobi!(A::Array{Float64}; ɛ=1e-10, maxiter=1e8)
    R = eye(A)
    iterations = 0
    maxnondiag = offdiagmax(A)[1]
    while (maxnondiag > ɛ && iterations ≤ maxiter)
        @inbounds maxnondiag, row, column = offdiagmax(A)
        @inbounds jacobirotate!(A, R, row, column)
        iterations += 1
    end
    return R
end

# Skriv om for å ikke bruke offdiag:
# gå gjennom hele matrisen én gang,
# regn ut normen, hvis ikke null, gå gjennom alle
# på nytt
function offdiagmax(x)
    max = -1.0
    maxrow = 0
    maxcol = 0
    rows, columns = size(x)
    for column in 1:columns
        for row in column+1:rows
            row == column && continue
            y = abs(x[row, column])
            if y ≥ max
                max = y
                maxrow, maxcol = row, column
            end
        end
    end
    max, maxrow, maxcol
end


function jacobirotate!(A::Array{Float64}, R::Array{Float64}, k, l)
    s, c = 0.0, 0.0
    if A[k, l] ≠ 0.0
        τ = (A[l, l] - A[k, k])/(2A[k, l])
        if τ ≥ 0
            t = 1/(τ + √(1 + τ^2))
        else
            t = -1/(-τ + √(1 + τ^2))
        end
        c = 1/√(1 + t^2)
        s = c⋅t
    else
        c = 1.0
        s = 0.0
    end

    aₖₖ = A[k, k]
    aₗₗ = A[l, l]
    A[k, k] = c^2⋅aₖₖ - 2.0c⋅s⋅A[k, l] + s^2⋅aₗₗ
    A[l, l] = s^2⋅aₖₖ + 2.0c⋅s⋅A[k, l] + c^2⋅aₗₗ
    A[k, l], A[l, k] = 0.0, 0.0
    for i in 1:size(A)[1]
        if k ≠ i ≠ l
            aᵢₖ = A[i, k]
            aᵢₗ = A[i, l]
            A[i, k] = c⋅aᵢₖ - s⋅aᵢₗ
            A[k, i] = A[i, k]
            A[i, l] = c⋅aᵢₗ + s⋅aᵢₖ
            A[l, i] = A[i, l]
        end
        rᵢₖ = R[i, k]
        rᵢₗ = R[i, l]
        R[i, k] = c⋅rᵢₖ - s⋅rᵢₗ
        R[i, l] = c⋅rᵢₗ + s⋅rᵢₖ
    end
end



end
