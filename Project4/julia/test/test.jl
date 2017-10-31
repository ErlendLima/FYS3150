include("../src/ising.jl")
using Ising
using Base.Test

@testset "Ising" begin
    @testset "Neighbours test" begin
        A = [1 2 3
             4 5 6
             7 8 9]
        neigh = neighbours(2, 3, A)
        @test neigh == [3, 5, 4, 9]
        neigh = neighbours(3, 3, A)
        @test neigh == [6, 8, 7, 3]
        neigh = neighbours(2, 2, A)
        @test neigh == [2, 4, 6, 8]
    end

    # @testset "Hamiltonian test" begin
    #     A = [1 1 -1
    #          -1 1 1
    #          -1 1 -1]
    #     @test hamiltonian(A, 2, 2, 1) == -2
    #     @test hamiltonian(A, 2, 2, 5) == -10
    #     @test hamiltonian(A, 3, 1, 1) == 0
    # end
end
