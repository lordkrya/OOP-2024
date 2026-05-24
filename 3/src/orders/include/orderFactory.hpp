#ifndef ORDERFACTORY_DEFENDER
#define ORDERFACTORY_DEFENDER

#include <map>

#include "baseOrder.hpp"
#include "wait.hpp"

namespace orders {

class OrderFactory: public BaseOrderFactory {
    public:
        OrderFactory() {
            registerOrder(0, []() { return std::make_unique<Wait>(); });
        }

        std::unique_ptr<Order> createOrder(unsigned int ID) override {
            auto it = orderCreators.find(ID);
            if (it != orderCreators.end()) {
                return it->second();
            }
            return nullptr;
        }

        void registerOrder(std::function<std::unique_ptr<Order>()> creator) {
            std::unique_ptr<Order> example = creator();
            orderCreators[example.getID()] = creator();
        }

    private:
        std::map<unsigned int, std::function<std::unique_ptr<Order>()>> orderCreators;
};

}

#endif
