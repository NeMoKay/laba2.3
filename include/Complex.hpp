#include <cmath>

struct Complex{
    double re, im;

    Complex(double r = 0, double i = 0) : re(r), im(i) {}

    Complex operator+(const Complex& other) const{
        return Complex(re + other.re, im + other.im);
    }

    Complex operator*(const Complex& other) const{
        return Complex(re * other.re - im * other.im, re * other.im + im * other.re);
    }

    bool operator!=(const Complex& other) const{
        return (re != other.re) || (im != other.im);
    }
    Complex& operator+=(const Complex& other){
        re += other.re;
        im += other.im;
        return *this;
    }
    bool operator==(const Complex& other) const{
        return (re == other.re) && (im == other.im);
    }
};


inline Complex sqrt(const Complex& c){
    return Complex(std::sqrt(c.re * c.re + c.im * c.im), 0);
}