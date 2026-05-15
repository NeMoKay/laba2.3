#pragma once

#include "iostream"
#include <cstddef>
#include <string>

#include "exceptions.hpp"

 


template <typename T>
class BitProxy{
private:

    T& start_data;
    size_t bitIndex;

public:
    BitProxy(T& value, size_t index);

    BitProxy& operator=(bool value);
    operator bool() const;
};




template <typename T>
BitProxy<T>::BitProxy(T& value, size_t index) : start_data(value), bitIndex(index){}

template <typename T>
BitProxy<T>& BitProxy<T>::operator=(bool value){
    if(value == 1){
        start_data = start_data | (static_cast<T>(1) << bitIndex);
    }
    
    else{
        start_data = start_data & ~(static_cast<T>(1) << bitIndex);
    }
    return *this;
}

template <typename T>
BitProxy<T>::operator bool() const{
    return (start_data >> bitIndex) & static_cast<T>(1);
}



template <typename T>
class Bit{
private:
    T value;
protected:
    void CheckIndex(size_t index);
public:

    Bit(); 
    Bit(T val);
    Bit(const Bit& operand);

    size_t GetBitCount();
    T GetValue() const; 
    void SetValue(T val); 

    bool operator[](size_t index) const;
    BitProxy<T> operator[](size_t index);
    
    operator bool() const; 
    Bit& operator=(const Bit& operand);
    Bit operator&(const Bit& operand) const;
    Bit operator|(const Bit& operand) const;
    Bit operator^(const Bit& operand) const;
    Bit operator~() const;
    bool operator==(const Bit& operand) const;
    bool operator!=(const Bit& operand) const;
};

//protected
template <typename T>
void Bit<T>::CheckIndex(size_t index){
    if(index >= sizeof(T) * 8){
        throw IndexOutOfRangeException(std::format("Индекс вне диапазона (индекс:{}, максимум:{})", index, sizeof(T) * 8 - 1));
    }
}

//public

template <typename T>
Bit<T>::Bit() : value(0){} 


template <typename T>
Bit<T>::Bit(T val) : value(val){}


template <typename T>
Bit<T>::Bit(const Bit& operand) : value(operand.value){}


template <typename T>
Bit<T>& Bit<T>::operator=(const Bit& operand){
    if(this != &operand){
        value = operand.value;
    }
    return *this;
}


template <typename T>
size_t Bit<T>::GetBitCount(){
    return sizeof(T) * 8;
}

template <typename T>
bool Bit<T>::operator[](size_t index) const{
    CheckIndex(index);
    return (value >> index) & static_cast<T>(1);
}

template <typename T>
BitProxy<T> Bit<T>::operator[](size_t index){
    CheckIndex(index);
    return BitProxy<T>(value, index);
}

template <typename T>
T Bit<T>::GetValue() const{ 
    return value;

}

template <typename T>
void Bit<T>::SetValue(T val){ 
    value = val; 
}

template <typename T>
Bit<T>::operator bool() const{ 
    return value != 0; 
}

template <typename T>
Bit<T> Bit<T>::operator&(const Bit& operand) const{
    return Bit(value & operand.value);
}

template <typename T>
Bit<T> Bit<T>::operator|(const Bit& operand) const{
    return Bit(value | operand.value);
}

template <typename T>
Bit<T> Bit<T>::operator^(const Bit& operand) const{
    return Bit(value ^ operand.value);
}

template <typename T>
Bit<T> Bit<T>::operator~() const{
    return Bit(~value);
}

template <typename T>
bool Bit<T>::operator==(const Bit& operand) const{
    return value == operand.value;
}

template <typename T>
bool Bit<T>::operator!=(const Bit& operand) const{
    return value != operand.value;
}