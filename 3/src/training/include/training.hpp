#ifndef TRAINING_DEFENDER
#define TRAINING_DEFENDER

#include <vector>

#include "map.hpp"
#include "division.hpp"

namespace training {

class Training {
    public:
        Training() = default;

        void setMap(map::Map&& other) noexcept { map::swap(map_, other); }

        //void setFirstGroup(const team::Division& other) { firstGroup = other; }
        void setFirstGroup(team::Division&& other) noexcept { team::swap(firstGroup, other); }

        //void setSecondGroup(const team::Division& other) { secondGroup = other; }
        void setSecondGroup(team::Division&& other) noexcept { team::swap(secondGroup, other); }

        map::Map& getMap() noexcept { return map_; }
        const team::Division& getFirstGroup() const noexcept { return firstGroup; }
        const team::Division& getSecondGroup() const noexcept { return secondGroup; }

        void synchronization() {
            map_.synchronizationFirst(firstGroup);
            map_.synchronizationSecond(secondGroup);
        }

        void doTick() {
            map_.doTick(firstGroup, secondGroup);
            firstGroup.doTick(map_);
            secondGroup.doTick(map_);
        }

    private:
        map::Map map_;
        unsigned int timeTick_;
        team::Division firstGroup;
        team::Division secondGroup;
};

}

#endif