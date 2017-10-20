include("vector3d.jl")
include("celestial.jl")
importall Vector3D
importall Celestial
using Base.Test

@testset "Vector3D" begin
    @testset "Constructor Test" begin
        v = Vec3(1.0, -2.0, 3.0)
        w = Vec3([1.0 -2.0 3.0])
        @test v == w
    end
    @testset "== Test" begin
        v = Vec3(1.0)
        w = Vec3(2.5)
        q = Vec3(1.0)
        p = Vec3(-1.0)
        @test !(v == w)
        @test v ≠ w
        @test v == q
        @test !(p == q)
        @test p ≠ q
    end
    @testset "- Test" begin
        v = Vec3(1.0)
        w = Vec3(2.5)
        result = Vec3(-1.5)
        @test result == v - w
    end
    @testset "+ Test" begin
        v = Vec3(1.0)
        w = Vec3(2.5)
        result = Vec3(3.5)
        @test result == v + w
        @test result ≠ v - w
    end
    @testset "* Test" begin
        v = Vec3(1.0, 2.0, 3.5)
        w = Vec3(2.5, 1.0, 2.0)
        q = Vec3(5.0, 2.0, 4.0)
        result = Vec3(2.5, 2.0, 7.0)
        @test result == v*w
        @test 2*w == q
    end
    @testset "/ Test" begin
        v = Vec3(1.0, 2.0, 3.5)
        w = Vec3(2.5, 1.0, 2.0)
        q = Vec3(0.5, 1.0, 3.5/2.0)
        result = Vec3(2.5, 0.5, 2.0/3.5)
        @test result ≈ w/v
        @test q ≈ v/2.0
        @test !(result ≈ w/7)
    end
    @testset "Norm Test" begin
        v = Vec3(1.0, 2.0, 3.5)
        w = Vec3(3.5, 1.0, 2.0)
        q = Vec3(0)
        @test 0 == norm(q)
        @test √17.25 ≈ norm(v)
        @test √17.25 ≈ norm(v)
        @test √3 ≈ norm(Vec3(1))
    end
    @testset "tomatrix Test" begin
        @test [1.0 3.4 -1.0] == tomatrix(Vec3(1.0, 3.4, -1.0))
    end
end

@testset "Celestial" begin
    body1 = CelestialBody(2)
    body2 = CelestialBody(2)
    body3 = CelestialBody(2)
    init!(body1, Vec3([1.0 2.0 3.0]), Vec3(4.0, 5.0, 6.0), 7.0)
    init!(body2, [1.0 2.0 3.0], Vec3([4.0 5.0 6.0]), 7.0)
    init!(body3, Vec3(1.0), Vec3(2.0), 1000.0)
    @testset "Init Test" begin
        @test body1.pos == body2.pos
        @test body1.vel == body2.vel
        @test body1.mass == body2.mass
    end
    @testset "Distance Test" begin
        @test distance(body1, body2, 1) == 0
        @test distance(body1, body3, 1) == √5
    end
end
