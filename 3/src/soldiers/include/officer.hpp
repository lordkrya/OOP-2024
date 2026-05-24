#ifndef OFFICER_DEFENDER
#define OFFICER_DEFENDER

#include <memory>

#include "specType.hpp"
#include "battalion.hpp"

namespace team {struct Battalion;}

namespace soldiers {

struct AttributeBattalions {
    AttributeBattalions(): ID_(0), level_(0) {}
    AttributeBattalions(unsigned int ID): ID_(ID), level_(1) {}

    AttributeBattalions& operator = (const AttributeBattalions& other) noexcept {
        ID_ = other.ID_;
        level_ = other.level_;
        return *this;
    };

    unsigned int ID_;
    int level_;
    std::weak_ptr<team::Battalion> ptr_;
};

class Officer: public SpecType {
    public:
        Officer(): attribute_() {}
        void changeAttribute(unsigned int ID);
        void changeTarget();

        inline unsigned int getType() const noexcept;

    private:
        AttributeBattalions attribute_;
};

}

#endif