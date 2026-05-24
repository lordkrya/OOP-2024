#include "leader.hpp"
#include "expectation.hpp"

namespace soldiers {

void Leader::changeInst(std::shared_ptr<instructions::Instruction>&& inst) {
    inst_ = std::move(inst);
}

const std::weak_ptr<instructions::Instruction> Leader::getInst() const {
    return inst_;
}

inline unsigned int Leader::getType() const noexcept {return 4;}

}