#pragma once

#include <string>
#include <iostream>
#include <exception>
#include <cstdlib>


class Exception{
private:
    std::string error;
public:
    explicit Exception(const char* message){
        if (message == nullptr){
            error = "";
        } 
        else{
            error = message;
        }
    }

    explicit Exception(const std::string& message){
        error = message;
    }

    const char* what() const noexcept{
        return error.c_str();
    }
};

class IndexOutOfRangeException : public Exception{
public:
    explicit IndexOutOfRangeException(const char* message) : Exception(message){}
    explicit IndexOutOfRangeException(const std::string& message) : Exception(message){}
};

class EmptySequenceException : public Exception{
public:
    explicit EmptySequenceException(const char* message) : Exception(message){}
    explicit EmptySequenceException(const std::string& message) : Exception(message){}
};

class InvalidSizeException : public Exception{
public:
    explicit InvalidSizeException(const char* message) : Exception(message){}
    explicit InvalidSizeException(const std::string& message) : Exception(message){}
};

class NullPtrException : public Exception{
public:
    explicit NullPtrException(const char* message) : Exception(message){}
    explicit NullPtrException(const std::string& message) : Exception(message){}
};

class LengthMismatchException : public Exception{
public:
    explicit LengthMismatchException(const char* message) : Exception(message){}
    explicit LengthMismatchException(const std::string& message) : Exception(message){}
};


inline void Terminate(){
    try{
        throw;
    }
    catch (const Exception& error){
        std::cerr << std::endl << "[ОШИБКА] " << error.what() <<  std::endl;
    }
    std::abort();
}


class AutoExceptionOn{
public:
    AutoExceptionOn(){
        std::set_terminate(Terminate);
    }
};

inline AutoExceptionOn AutoHandlerInstance;