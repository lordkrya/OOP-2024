#ifndef BASEOREDER_DEFENDER
#define BASEOREDER_DEFENDER

#include <memory>
#include <functional>

namespace orders {

class Order {
    public:
        virtual void executeOrder() const = 0;
        virtual unsigned int getID() const noexcept = 0;
        virtual std::string getName() const noexcept = 0;
        virtual ~Order() = default;
};

class BaseOrderFactory {
    public:
        virtual std::unique_ptr<Order> createOrder(unsigned int ID) = 0;
        virtual void registerOrder(std::function<std::unique_ptr<Order>()> creator) = 0;
        virtual ~BaseOrderFactory() = default;
};

}

#endif