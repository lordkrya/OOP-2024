#include "signals/binarySignal.h"

namespace signals {

size_t BinarySignal::signalSearchIndex(unsigned long int time, unsigned long int &now) const {
    size_t i = 0;
    now = 0;
    while (i < size_) {
        if ((now += signs_[i].getTime()) > time) return i;
        i++;
    }
    throw std::logic_error("Время превышает длительность сигнала");
}


BinarySignal::BinarySignal(const BinarySignal &signs): 
    signs_(new SignalStatus[signs.capacity_]), size_(signs.size_), capacity_(signs.capacity_) {
    
    std::copy(signs.signs_, signs.signs_ + size_, signs_);
}

BinarySignal::BinarySignal(BinarySignal &&signs) noexcept: signs_(signs.signs_), size_(signs.size_), capacity_(signs.capacity_) {
    signs.signs_ = nullptr;
    signs.size_ = 0;
    signs.capacity_ = 0;
}


BinarySignal::operator bool() const {
    return size_;
}


size_t BinarySignal::getSize() {
    return size_;
}

size_t BinarySignal::getCapacity() {
    return capacity_;
}


BinarySignal &BinarySignal::invert() {
    std::transform(signs_, signs_ + size_, signs_, [](SignalStatus sign){ return ~sign;});
    return *this;
}

BinarySignal &BinarySignal::insert(const BinarySignal &signs, unsigned long int time) {
    unsigned long int now = 0;
    size_t i = signalSearchIndex(time, now);

    if (size_ + signs.size_ + 1 > capacity_) {
        signs_ = utilities::resize<SignalStatus>(signs_, capacity_, (size_ + signs.size_) * 2);
        capacity_ = (size_ + signs.size_) * 2;
    }

    if (now - signs_[i].getTime() == time) {
        std::move_backward(signs_ + i, signs_ + size_, signs_ + size_ + signs.size_);
        std::copy(signs.signs_, signs.signs_ + signs.size_, signs_ + i);
        size_ += signs.size_;

    } else {
        std::move_backward(signs_ + i + 1, signs_ + size_, signs_ + size_ + signs.size_ + 1);
        std::copy(signs.signs_, signs.signs_ + signs.size_, signs_ + i + 1);

        signs_[i].setTime(signs_[i].getTime() - now + time);
        signs_[i+signs.size_+1] = SignalStatus(signs_[i].getStatus(), now - time);
        size_ += signs.size_ + 1;
    }
    return *this;
}

BinarySignal &BinarySignal::deleteFragment(unsigned long int time, unsigned long int duration) { // 0011 10 0011 0011
    unsigned long int now = 0;
    size_t i = signalSearchIndex(time, now), startI = i;
    bool flag = false;

    if (now != time + signs_[i].getTime()) {
        signs_[i].changeTime(-std::min(now - time, duration));
        if (duration > now - time) duration -= (now - time);
        else duration = 0;
        i++;
        flag = true;
    }

    while ((i < size_) && (signs_[i].getTime() <= duration)) {
        duration -= signs_[i].getTime();
        i++;
    }
    signs_[i].changeTime(-duration);

    if (flag) {
        std::move(signs_ + i, signs_ + size_, signs_ + startI + 1);
        size_ -= (i - startI - 1);
    } else {
        std::move(signs_ + i, signs_ + size_, signs_ + startI + 0);
        size_ -= (i - startI - 0);
    }
    return *this;
}


BinarySignal &BinarySignal::operator= (const BinarySignal &signs) {
    BinarySignal term(signs);
    swap(*this, term);
    return *this;
}

BinarySignal &BinarySignal::operator= (BinarySignal &&signs) noexcept {
    swap(*this, signs);
    return *this;
}

BinarySignal &BinarySignal::operator+= (const BinarySignal &signs) {
    if (size_ + signs.size_ > capacity_) {
        signs_ = utilities::resize<SignalStatus>(signs_, capacity_, std::max(capacity_, signs.capacity_) * 2);
        capacity_ = std::max(capacity_, signs.capacity_) * 2;
    }

    std::copy(signs.signs_, signs.signs_ + signs.size_, signs_ + size_);
    size_ += signs.size_;
    return *this;
}

BinarySignal BinarySignal::operator* (unsigned int a) const {
    BinarySignal newSigns = BinarySignal(*this); 

    newSigns.signs_ = utilities::resize<SignalStatus>(newSigns.signs_, capacity_, capacity_ * a);
    newSigns.capacity_ *= a;
    size_t startSize = size_;
    
    for (unsigned int i = 1; i < a; i++) {
        std::copy(signs_, signs_ + size_, newSigns.signs_ + newSigns.size_);
        newSigns.size_ += startSize;
    }
    return newSigns;
}

bool BinarySignal::operator[] (unsigned long int time) const {
    unsigned long int now = 0;
    size_t i = signalSearchIndex(time, now);
    return signs_[i].getStatus();
}


void swap(BinarySignal &first, BinarySignal &second) {
    std::swap(first.capacity_, second.capacity_);
    std::swap(first.size_, second.size_);
    std::swap(first.signs_, second.signs_);
}


std::istream &operator>> (std::istream &fin, BinarySignal &signs) {
    unsigned int count;
    signs.size_ = 0;
    
    fin >> count;
    while(fin.good() && count > signs.size_) {
        if (signs.size_ >= signs.capacity_) {
            signs.signs_ = utilities::resize<SignalStatus>(signs.signs_, signs.capacity_, signs.capacity_ * 2);
            signs.capacity_ = signs.capacity_ * 2;
        }
        
        fin >> signs.signs_[signs.size_];
        if (fin.good()) signs.size_++;
    }
    return fin;
}

std::ostream &operator<< (std::ostream &fout, const BinarySignal &sign) {
    std::string str = format(sign);
    fout << str;
    return fout;
}


std::string format(const BinarySignal &sign) {
    std::string str;
    for (size_t i = 0; i < sign.size_; i++) {
        str.append(signals::formatStatus(sign.signs_[i]));

        if (i+1 < sign.size_) {
            if (sign.signs_[i].getStatus() > sign.signs_[i+1].getStatus()) str.append("\\");
            if (sign.signs_[i].getStatus() < sign.signs_[i+1].getStatus()) str.append("/");
        }
    }
    return str;
}

}
