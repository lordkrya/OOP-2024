#ifndef MAP_DEFENDER
#define MAP_DEFENDER

#include <vector>
#include <iostream>

#include "field.hpp"
#include "division.hpp"
#include "battalion.hpp"

namespace team {
    class Battalion;
    class Division;
}

namespace map {

class Field;

class Map {
    public:
        Map() = default;

        void setName(std::string&& str) noexcept { name_ = std::move(str); }
        void setSizeX(size_t x) noexcept { sizeX_ = x; }
        void setSizeY(size_t y) noexcept { sizeY_ = y; }

        const std::string& getName() const noexcept { return name_; }
        size_t getSizeX() const noexcept { return sizeX_; }
        size_t getSizeY() const noexcept { return sizeY_; }
        const std::vector<std::vector<Field>>& getSquares() const noexcept { return squares_; }

        void clear() noexcept { squares_.clear(); }

        void clearFirstGroup() noexcept;

        void clearSecondGroup() noexcept;

        void resize() {
            squares_.resize(sizeX_);
            for (auto& it: squares_) {
                it.resize(sizeY_);
            }
        }

        Field& operator () (size_t x, size_t y) {
            return squares_[x][y];
        }

        const Field& operator () (size_t x, size_t y) const {
            return squares_[x][y];
        }


        void synchronizationFirst(team::Division& firstGroup);

        void synchronizationSecond(team::Division& secondGroup);

        void synchronizationBattle();

        void doTick(team::Division& firstGroup, team::Division& secondGroup);

        friend void swap(Map& first, Map& second) noexcept;

        constexpr static unsigned int metersInSquare = 1000;

    private:
        std::string name_;
        size_t sizeX_;
        size_t sizeY_;
        std::vector<std::vector<Field>> squares_;
};

void swap(Map& first, Map& second) noexcept;

}

#endif
