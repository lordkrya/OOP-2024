#ifndef LEADER_DEFENDER
#define LEADER_DEFENDER

#include "officer.hpp"

namespace soldiers {

class Leader: public Officer {
    public:
        Leader(): Officer(), inst_() {}
        void changeInst(std::shared_ptr<instructions::Instruction>&& inst);
        const std::weak_ptr<instructions::Instruction> getInst() const;

        inline unsigned int getType() const noexcept;

    private:
        std::shared_ptr<instructions::Instruction> inst_;
};

}

#endif