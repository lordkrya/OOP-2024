#ifndef FIRE_DEFENDER
#define FIRE_DEFENDER

#include <iostream>

#include "baseOrder.hpp"

namespace orders {

class Fire: public Order {
    public:
        Fire(): efficiency_(1.0) {};

        void executeOrder() const {
            std::cout << "fire" <<std::endl;
        }

        inline unsigned int getID() const noexcept {
            return 1;
        }

        inline std::string getName() const noexcept {
            return "Fire";
        }

    private:
        float efficiency_;
};

}

#endif