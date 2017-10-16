module Vector3D
export Vec3, +, -, *, ==, ≠, ≈, norm, tomatrix
import Base: -, +, *, /, ==, ≠, ≈

mutable struct Vec3{T <: Real}
    x::T
    y::T
    z::T
end
Vec3(x::T) where {T<:Real} = Vec3(x, x, x)
Vec3(a::Array{T, 2}) where {T} = Vec3(a[1], a[2], a[3])

# Implemented basic arithmetic
-(v1::Vec3, v2::Vec3) = Vec3(v1.x - v2.x,
                             v1.y - v2.y,
                             v1.z - v2.z)
*(c, v::Vec3) = Vec3(c*v.x, c*v.y, c*v.z)
*(v::Vec3, c) = *(c, v)
*(v1::Vec3, v2::Vec3) = Vec3(v1.x * v2.x,
                             v1.y * v2.y,
                             v1.z * v2.z)

+(v1::Vec3, v2::Vec3) = Vec3(v1.x + v2.x,
                             v1.y + v2.y,
                             v1.z + v2.z)
/(v1::Vec3, v2::Vec3) = Vec3(v1.x / v2.x,
                             v1.y / v2.y,
                             v1.z / v2.z)
/(v::Vec3, c) = Vec3(v.x / c, v.y / c, v.z / c)

# Boolean operators
==(v1::Vec3, v2::Vec3) = v1.x == v2.x && v1.y == v2.y && v1.z == v2.z
≠(v1::Vec3, v2::Vec3) = !(v1 == v2)
≈(v1::Vec3, v2::Vec3) = v1.x ≈ v2.x && v1.y ≈ v2.y && v1.z ≈ v2.z

# Implement ordinary methods
tomatrix(v::Vec3) = [v.x v.y v.z]
norm(v::Vec3) = √(v.x^2 + v.y^2 + v.z^2)
end
