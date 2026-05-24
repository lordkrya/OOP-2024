#ifndef SOLDIERSTATUS_DEFENDER
#define SOLDIERSTATUS_DEFENDER

#include "soldier.hpp"

namespace soldiers {

class Soldier;

enum Rang {
    undefined,
    squaddie,
    sergeant,
    ensign,
    lieutenant,
    captain,
};

enum Skill {
    shooter,
    doctor,
    scout,
    sapper,
    signalman,
};

enum VitalSign {
    efficient = 0,
    injured = 1,
    dead = 2,
};

}

#endif