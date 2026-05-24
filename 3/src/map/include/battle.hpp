#ifndef BATTLE_DEFENDER
#define BATTLE_DEFENDER

#include "field.hpp"

namespace map {

class Field;

class Battle {

public:
    Battle(Field& field): field_(field) {}

    void doTick() {

    }

private:
    Field& field_;
};

}

#endif