#include "specType.hpp"

namespace soldiers {

void SpecType::action() {}
orders::Orders* SpecType::getActions() noexcept { return nullptr; }

void SpecType::changeAttribute(unsigned int ID) { ID++; }
void SpecType::changeTarget() {}

void SpecType::changeInst(std::shared_ptr<instructions::Instruction>&& inst) { *inst; };
const std::weak_ptr<instructions::Instruction> SpecType::getInst() const {
    return {};
}

}
