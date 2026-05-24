#include "division.hpp"

namespace team {

//Division::Division(const Division& other): battalions_(other.battalions_), staff_(other.staff_) {}

Division::Division(Division&& other) noexcept: battalions_(std::move(other.battalions_)), staff_(std::move(other.staff_)) {}

const vect_bat& Division::getBattalions() noexcept {
    return battalions_;
}
const std::shared_ptr<Battalion>& Division::getStaff() noexcept {
    return *staff_;
}

void Division::setBattalions(vect_bat&& battalions) noexcept {
    battalions_ = std::move(battalions);
}

void Division::doTick(map::Map& map_) {
    for (auto& it: battalions_) {
        (*it).doTick(map_);
    }
}

void swap(Division &first, Division &second) noexcept {
    std::swap(first.battalions_, second.battalions_);
    std::swap(first.staff_, second.staff_);
}

}