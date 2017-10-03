module Jacobi
export offdiagmax, jacobirotate!, jacobi!

#=
This module provides different methods to find the eigenvalues for
matrices. These are taken verbatim from
https://compphysics.github.io/ComputationalPhysics/doc/pub/eigvalues/pdf/eigvalues-print.pdf
and only adapted to Julia.
=#

function jacobi!(A::Array{Float64}; ɛ=1e-10, maxiter=1e8)
    # Sets up the algorithm and begins a loop finding the max non diagonal element
    # before performing Given's rotations. It terminates if all non diagonal elements
    # are less than ε or if maxiter number of iterations are reached.
    # @Input: A, the array to find the eigenvalues of. To save memory, the
    #         array is overwritten with the eigenvalues along the diagonal.
    #         ε, tolerance of non diagonal elements
    #         maxiter, maximum number of iterations before breaking the loop
    # @Return: The eigenvectors and the number of iterations that was needed
    #          to make all off diagonals less than ε
    R = eye(A)
    iterations = 0
    maxnondiag = offdiagmax(A)[1]
    while (maxnondiag > ɛ && iterations ≤ maxiter)
        @inbounds maxnondiag, row, column = offdiagmax(A)
        @inbounds jacobirotate!(A, R, row, column)
        iterations += 1
    end
    R, iterations
end

function offdiagmax(x)
    # Finds the maximum off diagonal element in the lower quadrant
    # @Input: x, the matrix to find
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
            t = 1.0/(τ + √(1 + τ^2))
        else
            t = -1.0/(-τ + √(1 + τ^2))
        end
        c = 1.0/√(1 + t^2)
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
    for i in 1:size(A, 1)
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
