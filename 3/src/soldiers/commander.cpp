#include "commander.hpp"
#include "expectation.hpp"

namespace soldiers {

void Commander::changeInst(std::shared_ptr<instructions::Instruction>&& inst) {
    inst_ = std::move(inst);
}

const std::weak_ptr<instructions::Instruction> Commander::getInst() const {
    return inst_;
}

inline unsigned int Commander::getType() const noexcept {return 2;}

}