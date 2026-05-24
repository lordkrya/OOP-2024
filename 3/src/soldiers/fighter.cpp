#include "fighter.hpp"

namespace soldiers {

void Fighter::action() {
    std::unique_ptr<orders::Order>& order = orders_.getOrder();
    (*order).executeOrder();
    orders_.nextOrder();
}

orders::Orders* Fighter::getActions() noexcept { return &orders_; }

inline unsigned int Fighter::getType() const noexcept {return 1;}

}