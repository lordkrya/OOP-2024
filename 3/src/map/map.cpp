#include "map.hpp"

namespace map {

void Map::clearFirstGroup() noexcept {
    for (size_t x = 0; x < sizeX_; x++) {
        for (size_t y = 0; y < sizeY_; y++) {
            squares_[x][y].clearFirst();
        }
    }
}

void Map::clearSecondGroup() noexcept {
    for (size_t x = 0; x < sizeX_; x++) {
        for (size_t y = 0; y < sizeY_; y++) {
            squares_[x][y].clearSecond();
        }
    }
}

void Map::synchronizationFirst(team::Division& firstGroup) {
    clearFirstGroup();
    for (const auto& it: firstGroup.getBattalions()) {
        const team::Location& pos = (*it).getPosition();
        squares_[pos.x_ / metersInSquare][pos.y_ / metersInSquare].insertFirst(it);
    }
}

void Map::synchronizationSecond(team::Division& secondGroup) {
    clearSecondGroup();
    for (const auto& it: secondGroup.getBattalions()) {
        const team::Location& pos = (*it).getPosition();
        squares_[pos.x_ / metersInSquare][pos.y_ / metersInSquare].insertSecond(it);
    }
}

void Map::synchronizationBattle() {
    for (size_t x = 0; x < sizeX_; x++) {
        for (size_t y = 0; y < sizeY_; y++) {
            Field& field = squares_[x][y];
            if (!field.emptyFirst() && !field.emptySecond()) {
                field.setBattle(std::make_unique<Battle>(field));
            } else {
                field.setBattle(nullptr);
            }
        }
    }
}

void Map::doTick(team::Division& firstGroup, team::Division& secondGroup) {
    /*for (const auto& it: secondGroup.getBattalions()) {
        const team::Location& pos = (*it).getPosition();
        if (squares_[pos.x_ / metersInSquare][pos.y_ / metersInSquare].eraseFirst(it)) {
            squares_[pos.x_ / metersInSquare][pos.y_ / metersInSquare].insertSecond(it);
        }

        squares_[pos.x_ / metersInSquare][pos.y_ / metersInSquare].insertSecond(it);
    }*/
    synchronizationFirst(firstGroup);
    synchronizationSecond(secondGroup);
    synchronizationBattle();
}

void swap(Map& first, Map& second) noexcept {
    std::swap(first.name_, second.name_);
    std::swap(first.sizeX_, second.sizeX_);
    std::swap(first.sizeY_, second.sizeY_);
    std::swap(first.squares_, second.squares_);
}

}
