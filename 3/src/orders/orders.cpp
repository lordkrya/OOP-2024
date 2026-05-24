#include "orders.hpp"
#include "wait.hpp"

namespace orders {

void Orders::addOrder(std::unique_ptr<Order>&& order) {
    arr_.push_back(std::move(order));
}

void Orders::delOrder(unsigned int ID) {
    for(auto it = arr_.begin(); it != arr_.end();){
        if((*it)->getID() == ID){
            it = arr_.erase(it);
        } else {
            it++;
        }
    }
}

void Orders::delAllOrder() {
    arr_.clear();
}

std::unique_ptr<Order>& Orders::getOrder() {
    if (arr_.empty()) {
        addOrder(std::make_unique<Wait>());
    }

    if (ptr_ >= arr_.size())
        ptr_ %= arr_.size();

    return arr_[ptr_];
}

void Orders::nextOrder() noexcept {
    ptr_++;
}

}
