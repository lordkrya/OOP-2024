#ifndef COMMANDER_DEFENDER
#define COMMANDER_DEFENDER

#include "fighter.hpp"
#include "baseInstruction.hpp"

namespace soldiers {

class Commander: public Fighter {
    public:
        Commander(): Fighter(), inst_() {}
        void changeInst(std::shared_ptr<instructions::Instruction>&& inst);
        const std::weak_ptr<instructions::Instruction> getInst() const;

        inline unsigned int getType() const noexcept;

    private:
        std::shared_ptr<instructions::Instruction> inst_;
};

}

#endif