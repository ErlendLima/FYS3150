module Jacobi
export offdiagmax, jacobi_rotate!, jacobi!

function jacobi!(A::Array)
    R = eye(A)
    ɛ = 1e-10
    iterations = 0
    max_iter = 100
    maxnondiag = offdiagmax(A)[1]
    while (maxnondiag > ɛ && iterations ≤ max_iter)
        maxnondiag, row, column = offdiagmax(A)
        jacobi_rotate!(A, R, row, column)
        iterations += 1
    end
    return R
end

function offdiagmax(x)
    max = -1.0
    max_row = 0
    max_col = 0
    rows, columns = size(x)
    for column in 1:columns
        for row in 1:rows
            row == column && continue
            y = abs(x[row, column])
            if y > max
                max = y
                max_row, max_col = row, column
            end
        end
    end
    max, max_row, max_col
end


function jacobi_rotate!(A::Array{}, R::Array{}, k, l)
    s, c = 0.0, 0.0
    if A[k, l] != 0.0
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

    a_kk = A[k, k]
    a_ll = A[l, l]
    A[k, k] = c^2⋅a_kk - 2c⋅s⋅A[k, l] + s^2⋅a_ll
    A[l, l] = s^2⋅a_kk + 2c⋅s⋅A[k, l] + c^2⋅a_ll
    A[k, l], A[l, k] = 0.0, 0.0
    for i in 1:size(A)[1]
        if i ≠ k && i ≠ l
            a_ik = A[i, k]
            a_il = A[i, l]
            A[i, k] = c⋅a_ik - s⋅a_il
            A[k, i] = A[i, k]
            A[i, l] = c⋅a_il + s⋅a_ik
            A[l, i] = A[i, l]
        end
        r_ik = R[i, k]
        r_il = R[i, l]
        R[i, k] = c⋅r_ik - s⋅r_il
        R[i, l] = c⋅r_il + s⋅r_ik
    end
end


end
