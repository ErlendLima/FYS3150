#pragma once
#include <iostream>


class vec3
{
public:
    vec3();
    vec3(double a, double b,double c);


    double components[3];
    double &operator()(int index){if(index <= 2 && index >= 0) return components[index]; else throw std::runtime_error("Inaldig index");}
    double &operator[](int index){return components[index];}
    //double &operator*(vec3 a,vec3 b){return a(0)*b(0) + a(1)*b(1) + a(2)*b(2);}

    vec3 &operator-=(const vec3 &rhs);
    vec3 &operator+=(const vec3 &rhs);
    vec3 &operator*=(const double &c);
    vec3 &operator/=(const double &c);
    vec3 &operator/=(const vec3 &rhs);
    vec3 &operator=(const vec3 &rhs);
    vec3 &operator=(const double &s);

    void normalize();

    void print() const;

    double x() {return components[0];}
    double y() {return components[1];}
    double z() {return components[2];}

    double lengthSquared() const;
    double length() const;

    vec3 randint(int min, int max);
    double cross2d(const vec3 &other);
};

inline vec3 operator-(vec3 lhs, vec3 rhs){
    lhs -= rhs;
    return lhs;
}

inline vec3 operator+(vec3 lhs, vec3 rhs){
    lhs += rhs;
    return lhs;
}

inline vec3 operator *(vec3 lhs, double s){
    lhs *= s;
    return lhs;
}

inline vec3 operator *(double s,vec3 rhs){
    rhs *= s;
    return rhs;
}

inline vec3 operator /(vec3 lhs, double s){
    lhs /= s;
    return lhs;
}

inline vec3 operator /(double s,vec3 rhs){
    rhs /= s;
    return rhs;
}

inline vec3 operator /(vec3 lhs,vec3 rhs){
    lhs /= rhs;
    return lhs;
}

inline double operator *(vec3 lhs,vec3 rhs){
    return lhs[0]*rhs[0] + lhs[1]*rhs[1] + lhs[2]*rhs[2];
}
