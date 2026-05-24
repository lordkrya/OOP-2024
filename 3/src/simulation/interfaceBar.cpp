#include "interfaceBar.hpp"

namespace simulation {

void swap(InterfaceBar& first, InterfaceBar& second) noexcept {
    std::swap(first.bar_, second.bar_);
    std::swap(first.sprite_, second.sprite_);
    std::swap(first.active_, second.active_);
}

}
