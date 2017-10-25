#include "vec3.h"
#include <cmath>
#include <stdlib.h>
#include <iostream>

vec3::vec3()
{
    components[0] = 0;
    components[1] = 0;
    components[2] = 0;
}
vec3::vec3(double a, double b, double c)
{
    components[0] = a;
    components[1] = b;
    components[2] = c;
}


vec3 &vec3::operator-=(const vec3 &rhs)
{
    components[0] -= rhs.components[0];
    components[1] -= rhs.components[1];
    components[2] -= rhs.components[2];
    return *this;
}

vec3 &vec3::operator+=(const vec3 &rhs)
{
    components[0] += rhs.components[0];
    components[1] += rhs.components[1];
    components[2] += rhs.components[2];
    return *this;
}

vec3 &vec3::operator *=(const double &c){
    components[0] *= c;
    components[1] *= c;
    components[2] *= c;
    return *this;
}


vec3 &vec3::operator /=(const double &c){
    components[0] /= c;
    components[1] /= c;
    components[2] /= c;
    return *this;
}

vec3 &vec3::operator /=(const vec3 &rhs){
    components[0] /= rhs.components[0];
    components[1] /= rhs.components[1];
    components[2] /= rhs.components[2];
    return *this;
}

vec3 &vec3::operator=(const vec3 &rhs)
{
    components[0] = rhs.components[0];
    components[1] = rhs.components[1];
    components[2] = rhs.components[2];
    return *this;
}

vec3 &vec3::operator=(const double &s)
{
    components[0] = s;
    components[1] = s;
    components[2] = s;
    return *this;
}



vec3 vec3::randint(int min, int max)
{
    components[0] = rand()%(max-min + 1) + min;
    components[1] = rand()%(max-min + 1) + min;
    components[2] = rand()%(max-min + 1) + min;
    return *this;
}

void vec3::normalize(){
    *this = *this/length();
}

void vec3::print() const{
    std::cout << "[" << components[0] << "," << components[1] << "," << components[2] << "]" << std::endl;
}

std::ostream & operator<<(std::ostream &os, const vec3 &self){
    os << "[" << self.components[0] << ", " <<  self.components[1] << ", " << self.components[2] << "]";
    return os;
}

double vec3::lengthSquared() const
{
    return components[0]*components[0] + components[1]*components[1] + components[2]*components[2];
}

double vec3::length() const
{
   return sqrt(lengthSquared());
}

double vec3::cross2d(const vec3 &other){
    double x = components[0]*other.components[1];
    double y = -other.components[0]*components[1];
    return x+y;
}
