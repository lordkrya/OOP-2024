#ifndef INTERFACEELEM_DEFENDER
#define INTERFACEELEM_DEFENDER

#include <SFML/Graphics.hpp>

namespace simulation {

class InterfaceElem {
    public:
        InterfaceElem() = default;
        InterfaceElem(const sf::Sprite& sprite): sprite_(sprite) {}
        InterfaceElem(sf::Sprite&& sprite): sprite_(std::move(sprite)) {}

        sf::FloatRect getGlobalBounds() const { return sprite_.getGlobalBounds(); }

        virtual void setOrigin(float x, float y) { sprite_.setOrigin(x, y); }
        virtual void setPosition(float x, float y) { sprite_.setPosition(x, y); }
        virtual void setText(sf::String&& str) {}
        virtual std::string getString() const { return ""; }
        virtual sf::Event::EventType handleEvent(const sf::Event& event, sf::RenderWindow& window) = 0;
        virtual void draw(sf::RenderWindow& window) const = 0;

    protected:
        sf::Sprite sprite_;
};

}

#endif
