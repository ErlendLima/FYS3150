include("jacobi.jl")
using Jacobi
using Base.Test

@testset "Jacobi Tests" begin
    @testset "Offdiagmax Test" begin
        A = [4.0 3.0 2.1
             5.1 1.0 1.0
             3.1 4.3 9.0]
        B = [5.1 -9.1
             2.1 1.1]
        @test offdiagmax(A) == (5.1, 2, 1)
        @test offdiagmax(B) == (9.1, 1, 2)
    end
    @testset "Jacobi Method Test" begin
        A = [2 0 0
             0 3 4
             0 4 9.0]
        B = A
        R = jacobi!(A)
        @test sort(diag(A)) == sort(eigvals(B))
    end
end
