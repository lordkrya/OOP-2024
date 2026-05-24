#ifndef BASEINSTRUCTION_DEFENDER
#define BASEINSTRUCTION_DEFENDER

#include <string>
#include <memory>
#include <functional>

namespace instructions {

class Instruction {
    public:
        virtual void request() const = 0;
        virtual unsigned int getRegulation() const noexcept = 0;
        virtual std::string getName() const noexcept = 0;
        virtual ~Instruction() = default;
};

class BaseInstructionFactory {
    public:
        virtual std::weak_ptr<Instruction> createInstruction(unsigned int ID) = 0;
        virtual void registerInstruction(std::shared_ptr<Instruction> creator) = 0;
        virtual ~BaseInstructionFactory() = default;
};

}

#endif
