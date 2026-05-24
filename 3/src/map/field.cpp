#include "field.hpp"

namespace map {

void swap(Field& first, Field& second) noexcept {
    std::swap(first.attributes_, second.attributes_);
    std::swap(first.type_, second.type_);
    std::swap(first.battalionsFirst_, second.battalionsFirst_);
    std::swap(first.battalionsSecond_, second.battalionsSecond_);
}

}