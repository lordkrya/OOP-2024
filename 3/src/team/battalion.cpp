#include "battalion.hpp"

namespace team {

float generateRandomFloat(float minRange, float maxRange) {
    static std::random_device rd; // Ключ генерации
    static std::mt19937 gen(rd()); // Mersenne Twister генератор случайных чисел
    static std::uniform_real_distribution<float> distribution; // Создание равномерного распределения в диапазоне

    if (distribution.min() != minRange || distribution.max() != maxRange) {
        distribution.param(std::uniform_real_distribution<float>::param_type(minRange, maxRange));
    }
    return distribution(gen);
}

//Battalion::Battalion(const Battalion& other):
//    table_(other.table_),
//    commander_(other.commander_),
//    vector_ptr_at(other.vector_ptr_at),
//    position_(other.position_),
//    direction_(other.direction_)
//{}

Battalion::Battalion(Battalion&& other) noexcept: 
    table_(std::move(other.table_)),
    commander_(std::move(other.commander_)),
    attributes_(std::move(other.attributes_)),
    position_(std::move(other.position_)),
    direction_(std::move(other.direction_))
{}

const elem_sold& Battalion::appointCommander(table_sold::iterator commander) noexcept {
    commander_ = commander;
    return *commander;
}

void Battalion::changeDirection(map::Map& map_) {
    float x = map::Map::metersInSquare * map_.getSizeX();
    float y = map::Map::metersInSquare * map_.getSizeX();
    direction_ = Location(generateRandomFloat(0, x),
        generateRandomFloat(0, y));
}

const Location& Battalion::moveTo(map::Map& map_, float k) {
    if (Location::nearPos(position_, direction_)) {
        changeDirection(map_);
    }

    Location newPos = direction_ - position_;
    position_ += newPos.normalize() * k;
    return position_;
}

void Battalion::doTick(map::Map& map_) {
    if (map_(position_.x_ / map::Map::metersInSquare, position_.y_ / map::Map::metersInSquare).isBattle()) {

    } else {
        moveTo(map_, 10);
    }
}

void Battalion::setPosition(unsigned int x, unsigned int y) {
    position_ = Location(x, y);
    direction_ = Location(x, y);
}

void Battalion::setPosition(const Location &loc) {
    position_ = loc;
    direction_ = loc;
}

table_sold& Battalion::getTable() noexcept {
    return table_;
}
const elem_sold& Battalion::getCommander() const noexcept {
    return *commander_;
}
const Location& Battalion::getPosition() const noexcept {
    return position_;
}
const Location& Battalion::getDirection_() const noexcept {
    return direction_;
}

void swap(Location& first, Location& second) noexcept {
    std::swap(first.x_, second.x_);
    std::swap(first.y_, second.y_);
}

void swap(Battalion& first, Battalion& second) noexcept {
    table::swap(first.table_, second.table_);
    table::swap(first.commander_, second.commander_);
    std::swap(first.attributes_, second.attributes_);
    std::swap(first.position_, second.position_);
    std::swap(first.direction_, second.direction_);
}

}