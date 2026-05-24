#ifndef SPECFACTORY_DEFENDER
#define SPECFACTORY_DEFENDER

#include <map>

#include "specType.hpp"

namespace soldiers {

class SpecFactory: public SpecTypeFactory {
public:
    SpecTypeFactory() = default;

    std::unique_ptr<SpecType> createSpec(unsigned int ID) {
        auto it = specCreators.find(ID);
        if (it != specCreators.end()) {
            return it->second();
        }
        return nullptr;
    }

    void registerSpec(std::function<std::unique_ptr<SpecType>()> creator) = 0 {
            std::unique_ptr<SpecType> example = creator();
            specCreators[example.getType()] = creator();
    }

private:
    std::map<unsigned int, std::function<std::unique_ptr<SpecType>()>> specCreators;

};

}

#endif
