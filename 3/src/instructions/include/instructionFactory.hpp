#ifndef INSTRUCTIONFACTORY_DEFENDER
#define INSTRUCTIONFACTORY_DEFENDER

#include <map>

#include "baseInstruction.hpp"
#include "expectation.hpp"

namespace instructions {

class InstructionFactory: public BaseInstructionFactory {
    public:
        InstructionFactory() {
            registerOrder(0, []() { return std::make_shared<Expectation>(); });
        }

        std::weak_ptr<Instruction> createInstruction(unsigned int ID) override {
            auto it = instructionCreators.find(ID);
            if (it != instructionCreators.end()) {
                return it->second;
            }
            return {};
        }

        void registerInstruction(std::shared_ptr<Instruction>&& creator) {
            unsigned int ID = (*creator).getID();
            instructionCreators[ID] = std::move(creator);
        }

    private:
        std::map<unsigned int, std::shared_ptr<Instruction> instructionCreators;
};

}

#endif