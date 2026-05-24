#ifndef SPECTYPE_DEFENDER
#define SPECTYPE_DEFENDER

#include "orders.hpp"
#include "baseInstruction.hpp"

namespace soldiers {

class SpecType {
    public:
        virtual void action();
        virtual orders::Orders* getActions() noexcept;
        
        virtual void changeAttribute(unsigned int ID);
        virtual void changeTarget();

        virtual void changeInst(std::shared_ptr<instructions::Instruction>&& inst);
        virtual const std::weak_ptr<instructions::Instruction> getInst() const;

        virtual unsigned int getType() const noexcept = 0;
        virtual ~SpecType() = default;
};

class SpecTypeFactory {
    public:
        virtual std::unique_ptr<SpecType> createSpec(unsigned int ID) = 0;
        virtual void registerSpec(std::function<std::unique_ptr<SpecType>()> creator) = 0;
        virtual ~SpecTypeFactory() = default;
};

}

#endif