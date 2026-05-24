#ifndef OREDERS_DEFENDER
#define OREDERS_DEFENDER

#include <vector>
#include <memory>

#include "baseOrder.hpp"

namespace orders {

typedef std::vector<std::unique_ptr<Order>> vector_order;

class Orders {
    public:
        Orders(): arr_(), ptr_(0) {};

        void addOrder(std::unique_ptr<Order>&& order);
        void delOrder(unsigned int ID);
        void delAllOrder();

        std::unique_ptr<Order>& getOrder();
        void nextOrder() noexcept;

    private:
        vector_order arr_;
        vector_order::size_type ptr_;

};

}

#endif
