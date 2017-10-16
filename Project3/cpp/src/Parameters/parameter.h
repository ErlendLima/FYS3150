#ifndef PARAMETER_H
#define PARAMETER_H

#include <iostream>

struct NotImplementedException: public std::exception {
    const char* what () const throw () {
        return "Tried to get parameter with wrong type\n";
    }
};

struct UnsetException: public std::exception{
    const char* what () const throw (){
        return "Tried to get unset parameter\n";
    }
};

class ParameterInterface
{
public:
    virtual ~ParameterInterface(){};
    bool isSet() const {return flag;}
    void verify() const {if (!isSet()){throw UnsetException();}}
    virtual void read(const std::string& token) = 0;
protected:
    bool flag = false;
};

template <typename T>
class ParameterBase: public ParameterInterface
{
public:
    ParameterBase(){};
    virtual ~ParameterBase(){};
    virtual void read(const std::string& token) = 0;
    virtual T get() const{
        if (!isSet())
            throw UnsetException();
        return value;
    };
    virtual void setName(const std::string& name){this->name = name;}
protected:
    void set(const T& val){
        if (!isSet()){
            value = val;
            flag = true;
        } else {
            throw UnsetException();
        }
    };
private:
    T value;
    std::string name = "UnsetName";
    friend std::ostream & operator<<(std::ostream &os, const ParameterBase<T>& self){
        os << self.name << '\t' << self.get();
        return os;
    }
};

template <typename T>
class Parameter: public ParameterBase<T>
{
public:
    Parameter(){};
    Parameter(const std::string &name){ParameterBase<T>::setName(name);}
};

template <>
class Parameter<int>: public ParameterBase<int>
{
public:
    Parameter<int>(const std::string &name){setName(name);}
    void read(const std::string &token) override {set(static_cast<int>(std::stod(token)));}
};

template <>
class Parameter<double>: public ParameterBase<double>
{
public:
    Parameter<double>(const std::string &name){setName(name);}
    void read(const std::string &token) override {set(std::stod(token));}
};

template <>
class Parameter<std::string>: public ParameterBase<std::string>
{
public:
    Parameter<std::string>(const std::string &name){setName(name);}
    void read(const std::string &token) override {set(token);}
};

template <>
class Parameter<bool>: public ParameterBase<bool>
{
public:
    Parameter<bool>(const std::string &name){setName(name);}
    void read(const std::string &token) override {set(std::stoi(token));}
};
#endif /* PARAMETER_H */
