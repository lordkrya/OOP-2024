#include "officer.hpp"

namespace soldiers {

void Officer::changeAttribute(unsigned int ID) {
    attribute_ = AttributeBattalions(ID);
}

void Officer::changeTarget() {
    return;
}

inline unsigned int Officer::getType() const noexcept {return 3;}

}