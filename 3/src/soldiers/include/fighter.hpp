#ifndef FIGHTER_DEFENDER
#define FIGHTER_DEFENDER

#include "specType.hpp"

namespace soldiers {

class Fighter: public SpecType {
    public:
        Fighter(): orders_() {}
        void action();
        orders::Orders* getActions() noexcept;
    
        inline unsigned int getType() const noexcept;

    protected:
        orders::Orders orders_;
};

}

#endif