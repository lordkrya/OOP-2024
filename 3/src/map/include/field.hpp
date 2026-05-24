#ifndef FIELD_DEFENDER
#define FIELD_DEFENDER

#include <string>
#include <vector>

#include "battalion.hpp"
#include "battle.hpp"

namespace team { class Battalion; }

namespace map {

typedef std::vector<std::weak_ptr<team::Battalion>> vector_ptr_bat;

enum Region {
    plain,
    forest,
    hill,
    mountain,
};

struct FieldAttribute {
    FieldAttribute(): ID_(), level_() {}
    FieldAttribute(unsigned int ID, int level): ID_(ID), level_(level) {}

    unsigned int ID_;
    int level_;
};


class Field {
    public:
        Field() = default;

        Field(Field&& other) noexcept: attributes_(std::move(other.attributes_)),
        type_(other.type_),
        battalionsFirst_(std::move(other.battalionsFirst_)),
        battalionsSecond_(std::move(other.battalionsSecond_)) {}

        Field& operator = (Field&& other) noexcept {
            swap(*this, other);
            return *this;
        }

        const vector_ptr_bat& getBattalionsFirst() const noexcept {
            return battalionsFirst_;
        }
    
        const vector_ptr_bat& getBattalionsSecond() const noexcept {
            return battalionsSecond_;
        }

        bool emptyFirst() const noexcept {
            return battalionsFirst_.empty();
        }

        bool emptySecond() const noexcept {
            return battalionsSecond_.empty();
        }

        void setAttrubutes(std::vector<FieldAttribute>&& other) noexcept { attributes_ = std::move(other); }
        void setRegion(Region reg) noexcept { type_ = reg; }
        void setBattalionsFirst(vector_ptr_bat&& other) noexcept { battalionsFirst_ = std::move(other); }
        void setBattalionsSecond(vector_ptr_bat&& other) noexcept { battalionsSecond_ = std::move(other); }

        void insertFirst(const std::weak_ptr<team::Battalion>& b) {
            battalionsFirst_.push_back(b);
        }

        void insertSecond(const std::weak_ptr<team::Battalion>& b) {
            battalionsSecond_.push_back(b);
        }

        bool eraseFirst(const std::weak_ptr<team::Battalion>& elem) {
            auto it = std::find_if(battalionsFirst_.begin(), battalionsFirst_.end(),
            [&](const std::weak_ptr<team::Battalion>& wptr) {
                if (auto ptr1 = wptr.lock()) {
                    if (auto ptr2 = elem.lock()) {
                        return ptr1 == ptr2;
                    } else {
                        return false;
                    }
                } else
                    return false;
            });

            if (it != battalionsFirst_.end()) {
                battalionsFirst_.erase(it);
                return true;
            }
            return false;
        }

        bool eraseSecond(std::weak_ptr<team::Battalion>& elem) {
            auto it = std::find_if(battalionsSecond_.begin(), battalionsSecond_.end(),
            [&](const std::weak_ptr<team::Battalion>& wptr) {
                if (auto ptr1 = wptr.lock()) {
                    if (auto ptr2 = elem.lock()) {
                        return ptr1 == ptr2;
                    } else {
                        return false;
                    }
                } else
                    return false;
            });

            if (it != battalionsSecond_.end()) {
                battalionsSecond_.erase(it);
                return true;
            }
            return false;
        }

        void clearFirst() noexcept {
            battalionsFirst_.clear();
        }

        void clearSecond() noexcept {
            battalionsSecond_.clear();
        }

        void setBattle(std::unique_ptr<Battle>&& battle) {
            battle_ = std::move(battle);
        }

        bool isBattle() {
            return bool(battle_);
        }

        friend void swap(Field& first, Field& second) noexcept;

    private:
        std::vector<FieldAttribute> attributes_;
        Region type_;
        vector_ptr_bat battalionsFirst_;
        vector_ptr_bat battalionsSecond_;

        std::unique_ptr<Battle> battle_;
};

void swap(Field& first, Field& second) noexcept;

}

#endif