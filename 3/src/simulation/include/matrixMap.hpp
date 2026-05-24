#ifndef MATRIXMAP_DEFENDER
#define MATRIXMAP_DEFENDER

#include <SFML/Graphics.hpp>

namespace simulation {

class MatrixMap {
public:
    MatrixMap() = default;

    MatrixMap(MatrixMap&& other) noexcept: rect_(std::move(other.rect_)),
    position_(std::move(other.position_)), scale_(other.scale_) {}

    MatrixMap(const sf::Vector2f& size, const sf::Texture* texture, const sf::Vector2f& position = {0, 0}):
    rect_(size), position_(position) {
        rect_.setOrigin(size.x / 2, size.y / 2);
        rect_.setTexture(texture);
        rect_.setPosition(position_);
    }

    void setPosition(const sf::Vector2f& position) {
        position_ = position;
        rect_.setPosition(position_);
    }

    const sf::Vector2f& getPosition() const {
        return position_;
    }

    float getScale() const {
        return scale_;
    }

    sf::FloatRect getGlobalBounds() const {
        return rect_.getGlobalBounds();
    }

    void changePosition(float x, float y) {
        sf::Vector2f newPosition = {position_.x + x, position_.y + y};
        setPosition(newPosition);
    }

    void changePositionWithLimit(float x, float y) {
        sf::Vector2f newPosition = {position_.x + x, position_.y + y};
        if (newPosition.x > 0 && newPosition.y > 0) {
            if (newPosition.x < sf::VideoMode::getDesktopMode().width &&
                newPosition.y < sf::VideoMode::getDesktopMode().height) {
                setPosition(newPosition);
            }
        }
    }

    void zoomIn(float factor) {
        scale_ += factor;
        if (scale_ >= 0.2 && scale_ <= 5) {
            rect_.setScale(scale_, scale_);

        } else {
            scale_ -= factor;
        }
    }

    const sf::Vector2f& getSize() const{
      return rect_.getSize();
    }

    void setTexture(const sf::Texture& texture) {
        rect_.setTexture(&texture);
    }

    virtual void draw(sf::RenderWindow& window) {
        window.draw(rect_);
    }
    
    virtual std::optional<sf::Vector2f> handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
        sf::FloatRect bounds = rect_.getGlobalBounds();
        sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            if (bounds.contains(mousePosition)) {
                sf::Vector2f relativeMousePos = mousePosition - position_;
                return relativeMousePos;
            }
        }

        if (event.type == sf::Event::KeyPressed) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) changePositionWithLimit(0, 10);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) changePositionWithLimit(0, -10);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) changePositionWithLimit(10, 0);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) changePositionWithLimit(-10, 0);
        }
        if (event.type == sf::Event::MouseWheelScrolled) {
            float delta = event.mouseWheelScroll.delta;
            zoomIn(0.05 * delta);
        }

        return std::nullopt;
    }

protected:
    sf::RectangleShape rect_;
    sf::Vector2f position_;
    float scale_ = 1;
};

}


#endif
