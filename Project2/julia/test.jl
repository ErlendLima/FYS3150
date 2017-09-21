include("jacobi.jl")
include("schroedinger.jl")
using Jacobi
using Schroedinger
using Base.Test

@testset "Jacobi Tests" begin
    @testset "Offdiagmax Test" begin
        A = [4.0 3.0 2.1
             5.1 1.0 1.0
             3.1 4.3 9.0]
        B = [5.1 -9.1
             9.1 1.1]
        C = rand(50,50)
        C[23,5] = 1000
        @test offdiagmax(A) == (5.1, 2, 1)
        @test offdiagmax(B) == (9.1, 2, 1)
        @test offdiagmax(C) == (1000, 23, 5)
    end
    @testset "Jacobi Method Test" begin
        A = [2 0 0
             0 3 4
             0 4 9.0]
        A = [1.0 -1 0
             -1 2 -1
             0 -1 3]
        B = copy(A)
        R = jacobi!(A)
        @test sort(diag(A)) ≈ sort(eigvals(B))
    end
    @testset "Jacobi Small Test" begin
        A = [2.0 1.0;
             1.0 -2.0]
        B = copy(A)
        R = jacobi!(A)
        @test sort(diag(A)) ≈ sort(eigvals(B))
    end
    @testset "Jacobi Large Test" begin
        for N in 2:5
            A = reshape(collect(1.0:(N^2)), (N,N))
            A = Symmetric(A)
            B = full(A)
            R = jacobi!(B)
            @test sort(diag(B)) ≈ sort(eigvals(A))
        end
    end
    @testset "Jacobi Simple Symmetric Test" begin
        dv = [2, 2, 2, 2, 2, 2, 2, 2, 2.0, 2]
        ev = [1, 1, 1, 1, 1, 1, 1, 1, 1.0]
        A = SymTridiagonal(dv, ev)
        B = full(A)
        R = jacobi!(B)
        @test sort(diag(B)) ≈ sort(eigvals(A))
    end
    @testset "Jacobi 3 Sweep Test" begin
        A = [4.0 -30 60 -35
             -30 300 -675 20
             60 -675 1620 -1050
             -35 20 -1050 700]
        B = copy(A)
        R = jacobi!(A)
        @test sort(diag(A)) ≈ sort(eigvals(B))
    end
end


@testset "Schroedinger Tests" begin
    @testset "Solve test" begin
        summary(solve(0, 1, 10))
        @test true
    end
end
