#ifndef BATTALION_DEFENDER
#define BATTALION_DEFENDER

#include <cmath>
#include <random>
#include <iostream>

#include "soldier.hpp"
#include "orderedTable.hpp"
#include "officer.hpp"
#include "map.hpp"

namespace soldiers { struct AttributeBattalions; }

namespace map { class Map; }

namespace team {

struct SoldierStatus{
    SoldierStatus() = default;

    SoldierStatus(SoldierStatus&& other) noexcept: person_(std::move(other.person_)), status_(other.status_) {}
    //SoldierStatus(const SoldierStatus& other): person_(other.person_), status_(other.status_) {}

    SoldierStatus& operator = (SoldierStatus&& other) {
        swap(*this, other);
        return *this;
    }

    void swap(SoldierStatus& first, SoldierStatus& second) noexcept {
        soldiers::swap(first.person_, second.person_);
        std::swap(first.status_, second.status_);
    }

    soldiers::Soldier person_;
    soldiers::VitalSign status_;
};

typedef table::OrderedTable<unsigned int, SoldierStatus> table_sold;
typedef table::Elem<unsigned int, SoldierStatus> elem_sold;
typedef std::vector<std::weak_ptr<soldiers::AttributeBattalions>> vector_ptr_at;

struct Location {
    Location() = default;
    Location(float x, float y) noexcept: x_(x), y_(y) {}
    Location(const Location& other) noexcept: x_(other.x_), y_(other.y_) {}

    Location& operator = (const Location& other) noexcept {
        x_ = other.x_;
        y_ = other.y_;
        return *this;
    }
    Location operator - (const Location& other) {
        return Location(x_ - other.x_, y_ - other.y_);
    }
    Location& operator += (const Location& other) {
        x_ += other.x_;
        y_ += other.y_;
        return *this;
    }
    Location& operator * (float k) {
        x_ *= k;
        y_ *= k;
        return *this;
    }
    float norm() const noexcept {
        return sqrt(x_*x_ + y_*y_);
    }
    Location& normalize() {
        if (std::abs(x_) < epsilon && std::abs(y_) < epsilon) {
            x_ = 0;
            y_ = 0;
            return *this;
        }
        
        float norm_value = norm();
        x_ /= norm_value;
        y_ /= norm_value;
        return *this;
    }

    static bool nearPos(const Location &first, const Location &second) {
        if ((first.x_ - second.x_ < epsilon) && (first.y_ - second.y_ < epsilon)) {
            return true;
        }
        return false;
    }

    friend void swap(Location& first, Location& second) noexcept;

    float x_ = 0;
    float y_ = 0;
    constexpr static float epsilon = 5;
};

class Battalion {
    public:
        Battalion(): table_(), commander_(), position_(), direction_() {}

        //Battalion(const Battalion& other);
        Battalion(Battalion&& other) noexcept;

        void doTick(map::Map& map_);

        void changeDirection(map::Map& map_);

        const elem_sold& appointCommander(table_sold::iterator commander) noexcept;

        const Location& moveTo(map::Map& map_, float k);

        void setPosition(unsigned int x, unsigned int y);
        void setPosition(const Location &loc);

        table_sold& getTable() noexcept;
        const elem_sold& getCommander() const noexcept;
        const Location& getPosition() const noexcept;
        const Location& getDirection_() const noexcept;

        friend void swap(Battalion& first, Battalion& second) noexcept;

    private:
        table_sold table_;
        table_sold::iterator commander_;

        vector_ptr_at attributes_;
        Location position_;
        Location direction_;
};

}

#endif
