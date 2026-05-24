#ifndef DIVISION_DEFENDER
#define DIVISION_DEFENDER

#include "battalion.hpp"
#include "map.hpp"

namespace map { class Map; }

namespace team {

typedef std::vector<std::shared_ptr<Battalion>> vect_bat;

class Division {
    public:
        Division() = default;

        //Division(const Division& other);
        Division(Division&& other) noexcept;

        const vect_bat& getBattalions() noexcept;
        const std::shared_ptr<Battalion>& getStaff() noexcept;

        void setBattalions(vect_bat&& battalions) noexcept;

        void doTick(map::Map& map_);

        friend void swap(Division &first, Division &second) noexcept;

    private:
        vect_bat battalions_;
        vect_bat::iterator staff_;
};

void swap(Division &first, Division &second) noexcept;

}

#endif