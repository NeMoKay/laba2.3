#pragma once

#include "sequence.hpp"
#include "bit.hpp"
#include "iostream"
#include "Dynamic_Array.hpp"
#include <cstddef>
#include <string>

#include "exceptions.hpp"
 

template <typename T>
class BitSequence : public Sequence<Bit<T>>{
private:
    DynamicArray<T>* data;
    size_t bits_in_T;
    size_t length_bits;

    size_t GetByteIndex(size_t bitIndex) const;
    size_t Get_i_bit_in_byte(size_t bitIndex) const;

    void SetBit(size_t bitIndex, bool value);
    bool GetBit(size_t bitIndex) const;

protected:
    virtual BitSequence<T>* Clone() const;
    virtual BitSequence<T>* Instance();

    void AppendInternal(Bit<T> item);
    void PrependInternal(Bit<T> item);
    void InsertAtInternal(Bit<T> item, size_t index);
    void ConcatInternal(Sequence<Bit<T>>* list);

public:
    BitSequence();
    BitSequence(Bit<T>* new_items, size_t count);
    BitSequence(const BitSequence<T>& operand);

    Bit<T> GetFirst() const override;
    Bit<T> GetLast() const override;
    Bit<T> Get(size_t index) const override;
    size_t GetLength() const override;

    BitSequence<T>* GetSubsequence(size_t startIndex, size_t endIndex) const override;
    BitSequence<T>* Append(Bit<T> item) override;
    BitSequence<T>* Prepend(Bit<T> item) override;
    BitSequence<T>* InsertAt(Bit<T> item, size_t index) override;
    BitSequence<T>* Concat(Sequence<Bit<T>>* list) override;

    BitSequence<T> operator&(const BitSequence<T>& operand) const;
    BitSequence<T> operator|(const BitSequence<T>& operand) const;
    BitSequence<T> operator^(const BitSequence<T>& operand) const;
    BitSequence<T> operator~() const;

    ~BitSequence();
};


//private
template <typename T>
size_t BitSequence<T>::GetByteIndex(size_t bitIndex) const{
    return bitIndex / bits_in_T;

}

template <typename T>
size_t BitSequence<T>::Get_i_bit_in_byte(size_t bitIndex) const{

    size_t ost = bitIndex % bits_in_T;
    return (bits_in_T - 1) - ost;
}

template <typename T>
void BitSequence<T>::SetBit(size_t bitIndex, bool value){
    size_t byte_index = GetByteIndex(bitIndex);
    size_t bit_in_byte_index = Get_i_bit_in_byte(bitIndex);
    T byte_in_arr = data->Get(byte_index);
    T mask = static_cast<T>(1) << bit_in_byte_index;
    if(value){
        data->Set(byte_index, byte_in_arr | mask);

    } 
    else{
        data->Set(byte_index, byte_in_arr & ~mask);
    }
}


template <typename T>
bool BitSequence<T>::GetBit(size_t bitIndex) const{
    size_t byte_index = GetByteIndex(bitIndex);
    size_t bit_in_byte_index = Get_i_bit_in_byte(bitIndex);

    T byte_in_arr = data->Get(byte_index);
    return (byte_in_arr >> bit_in_byte_index) & static_cast<T>(1);
}


//protected

template <typename T>
BitSequence<T>* BitSequence<T>::Clone() const{
    BitSequence<T>* copy = new BitSequence<T>(*this);
    return copy;
}

template <typename T>
BitSequence<T>* BitSequence<T>::Instance(){
    return this;
}


template <typename T>
void BitSequence<T>::AppendInternal(Bit<T> item){
    bool val = bool(item);
    size_t oldLen = length_bits;

    size_t oldByteCount = (oldLen  + bits_in_T - 1) / bits_in_T;
    size_t newByteCount = (oldLen  + bits_in_T) / bits_in_T;


    if(newByteCount > oldByteCount){
        data->Resize(newByteCount);
        data->Set(newByteCount - 1, 0);
    }

    SetBit(oldLen, val);
    length_bits++;
}

template <typename T>
void BitSequence<T>::PrependInternal(Bit<T> item){
    InsertAtInternal(item, 0);
}



template <typename T>
void BitSequence<T>::InsertAtInternal(Bit<T> item, size_t index){
    if(index > length_bits){
        throw IndexOutOfRangeException(std::format("Ошибка индекса (индекс:{}, максимум:{})", index, length_bits));
    }
    bool val = bool(item);
    size_t oldLen = length_bits;
    if(index == oldLen){
        AppendInternal(item);
        return;
    }

    size_t oldByteCount = (oldLen  + bits_in_T - 1) / bits_in_T;
    size_t newByteCount = (oldLen  + bits_in_T) / bits_in_T;

    if(newByteCount > oldByteCount){
        data->Resize(newByteCount);
        data->Set(newByteCount - 1, 0);
    }

    for (size_t i = oldLen; i > index; --i){
        bool bitVal = GetBit(i - 1);
        SetBit(i, bitVal);
    }

    SetBit(index, val);
    length_bits++;
}

template <typename T>
void BitSequence<T>::ConcatInternal(Sequence<Bit<T>>* list){
    if(list == nullptr){
        return;
    }
    for (size_t i = 0; i < list->GetLength(); i++){
        this->AppendInternal(list->Get(i));
    }

}




//public



template <typename T>
BitSequence<T>::BitSequence() : data(new DynamicArray<T>), bits_in_T(sizeof(T) * 8), length_bits(0){}

template <typename T>
BitSequence<T>::BitSequence(Bit<T>* new_items, size_t count) : bits_in_T(sizeof(T) * 8){
    if(new_items == nullptr){

        throw NullPtrException("Указатель на массив битов не может быть nullptr");
    }


    size_t bytesNeeded = (count + bits_in_T - 1) / bits_in_T;
    data = new DynamicArray<T>(bytesNeeded);
    
    for(size_t i = 0; i < bytesNeeded; i++){
        data->Set(i, 0);
    }

    for(size_t i = 0; i < count; i++){
        bool val = bool(new_items[i]);
        SetBit(i, val);
    }

    length_bits = count;
}

template <typename T>
BitSequence<T>::BitSequence(const BitSequence<T>& operand) : data(new DynamicArray<T>(*(operand.data))), bits_in_T(operand.bits_in_T), length_bits(operand.length_bits){}

template <typename T>
Bit<T> BitSequence<T>::GetFirst() const{
    if(length_bits == 0){
        throw EmptySequenceException("Список пуст");
    }
    return Get(0);
}

template <typename T>
Bit<T> BitSequence<T>::GetLast() const{
    if(length_bits == 0){
        throw EmptySequenceException("Список пуст");
    }
    return Get(length_bits - 1);
}

template <typename T>
Bit<T> BitSequence<T>::Get(size_t index) const{
    if(index >= length_bits){
        throw IndexOutOfRangeException(std::format("Ошибка индекса (индекс:{}, размер:{})", index, length_bits));
    }
    bool isBitSet = GetBit(index);
    T correct_val;
    if(isBitSet == true){
        correct_val = static_cast<T>(1);
    } 
    else{
        correct_val = static_cast<T>(0);
    }
    return Bit<T>(correct_val);
}

template <typename T>
size_t BitSequence<T>::GetLength() const{
    return length_bits;
}

template <typename T>
BitSequence<T>* BitSequence<T>::GetSubsequence(size_t startIndex, size_t endIndex) const{
    if(endIndex < startIndex || startIndex >= length_bits || endIndex >= length_bits){
        throw IndexOutOfRangeException(std::format("Ошибка индекса (start:{}, end:{}, size:{})", startIndex, endIndex, length_bits));
    }

    BitSequence<T>* result = new BitSequence<T>;
    for(size_t i = startIndex; i <= endIndex; i++){
        result->Append(Get(i));
    }
    return result;
}

template <typename T>
BitSequence<T>* BitSequence<T>::Append(Bit<T> item){
    BitSequence<T>* arr = this->Instance();
    arr->AppendInternal(item);
    return arr;
}

template <typename T>
BitSequence<T>* BitSequence<T>::Prepend(Bit<T> item){
    BitSequence<T>* arr = this->Instance();
    arr->PrependInternal(item);
    return arr;
}

template <typename T>
BitSequence<T>* BitSequence<T>::InsertAt(Bit<T> item, size_t index){
    BitSequence<T>* arr = this->Instance();
    arr->InsertAtInternal(item, index);
    return arr;
}

template <typename T>
BitSequence<T>* BitSequence<T>::Concat(Sequence<Bit<T>>* list){
    if(list == nullptr){
        return this;
    }
    BitSequence<T>* arr = this->Instance();
    arr->ConcatInternal(list);
    return arr;
}

template <typename T>
BitSequence<T> BitSequence<T>::operator&(const BitSequence<T>& operand) const{
    if(this->length_bits != operand.length_bits){
        throw LengthMismatchException(std::format("Длины последовательностей не совпадают ({} !={})", this->length_bits, operand.length_bits));
    }
    BitSequence<T> result;
    for (size_t i = 0; i < this->length_bits; i++){
        result.Append(this->Get(i) & operand.Get(i));
    }
    return result;
}

template <typename T>
BitSequence<T> BitSequence<T>::operator|(const BitSequence<T>& operand) const{
    if(this->length_bits != operand.length_bits){
        throw LengthMismatchException(std::format("Длины последовательностей не совпадают ({} !={})", this->length_bits, operand.length_bits));
    }
    BitSequence<T> result;
    for (size_t i = 0; i < this->length_bits; i++){
        result.Append(this->Get(i) | operand.Get(i));
    }
    return result;
}

template <typename T>
BitSequence<T> BitSequence<T>::operator^(const BitSequence<T>& operand) const{
    if(this->length_bits != operand.length_bits){
        throw LengthMismatchException(std::format("Длины последовательностей не совпадают ({} !={})", this->length_bits, operand.length_bits));
    }
    BitSequence<T> result;
    for (size_t i = 0; i < this->length_bits; i++){
        result.Append(this->Get(i) ^ operand.Get(i));
    }
    return result;
}

template <typename T>
BitSequence<T> BitSequence<T>::operator~() const{
    BitSequence<T> result;
    for (size_t i = 0; i < length_bits; i++){
        bool current = bool(this->Get(i));
        result.Append(Bit<T>(static_cast<T>(!current)));
    }
    return result;
}

template <typename T>
BitSequence<T>::~BitSequence(){
    delete data;
}

template <typename T>
class MutableBitSequence : public BitSequence<T>{
protected:
    BitSequence<T>* Clone() const override{
        return new MutableBitSequence<T>(*this);
    }
    BitSequence<T>* Instance() override{
        return this;
    }
public:
    using BitSequence<T>::BitSequence;
};

template <typename T>
class ImmutableBitSequence : public BitSequence<T>{
protected:
    BitSequence<T>* Clone() const override{
        return new ImmutableBitSequence<T>(*this);
    }
    BitSequence<T>* Instance() override{
        return this->Clone();
    }
public:
    using BitSequence<T>::BitSequence;
};