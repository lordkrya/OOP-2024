#include "signals/signalStatus.h"

namespace signals {

SignalStatus::operator bool() const {
    return status_;
}

bool SignalStatus::getStatus() const {
    return status_;
}

unsigned long int SignalStatus::getTime() const {
    return time_;
}

void SignalStatus::setStatus(bool status) {
    status_ = status;
}

void SignalStatus::setTime(unsigned long int time) {
    if (time == 0) time_ = 1;
    else time_ = time;
}

void SignalStatus::changeTime(int long dtime) {
    if (dtime < 0 && (unsigned long int) (1 - dtime) > time_) time_ = 1;
    else {
        time_ += dtime;
        if (time_ == 0) time_ = 1;
    }
}


SignalStatus SignalStatus::operator~() const {
    return SignalStatus(!status_, time_);
}

std::istream &operator>>(std::istream &fin, SignalStatus &sign) {
    bool status;
    unsigned long int time;
    fin >> status >> time;
    if (fin.good()) {
        sign.setStatus(status);
        sign.setTime(time);
    }
    return fin;
}

std::ostream &operator<<(std::ostream &fout, const SignalStatus &sign) {
    fout << "Статус сигнала: " << sign.getStatus() << " | Длительность: " << sign.getTime() << std::endl;
    return fout;
}


std::string formatStatus(const SignalStatus &sign) {
    if (sign.getStatus()) return std::string(sign.getTime(), '#');
    else return std::string(sign.getTime(), '_');
}

}