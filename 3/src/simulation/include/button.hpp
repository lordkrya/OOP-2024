#ifndef BUTTON_DEFENDER
#define BUTTON_DEFENDER

#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>

#include "interfaceElem.hpp"

namespace simulation {

class Button: public InterfaceElem {
public:
    Button(): InterfaceElem(), onClick_(), isHovered_(false), isClicked_(false) {}

    //Конструктор, принимающий спрайт
    Button(sf::Sprite&& sprite, 
        const std::function<void(Button &this_)>& onClick = {}, 
        const std::function<void(Button &this_)>& preClick = {},
        const std::function<void(Button &this_)>& onLeave = {}):
        InterfaceElem(std::move(sprite)), 
        onClick_(onClick), 
        preClick_(preClick),
        onLeave_(onLeave),
        isHovered_(false), isClicked_(false) {
        constructorOrigin();
    }

    Button(Button&& other):
        InterfaceElem(std::move(other.sprite_)), text_(std::move(other.text_)),
        onClick_(other.onClick_), preClick_(other.preClick_),
        onLeave_(other.onLeave_), onHover_(other.onHover_),
        isHovered_(false), isClicked_(false) {}

    // Отрисовка кнопки
    void draw(sf::RenderWindow& window) const {
        window.draw(sprite_);
        window.draw(text_);
    }

    // Обработка событий мыши
    sf::Event::EventType handleEvent(const sf::Event& event, sf::RenderWindow& window) {
        sf::FloatRect bounds = sprite_.getGlobalBounds();
        sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        // Проверка, наведен ли курсор на кнопку
        if (bounds.contains(mousePosition)) {
            sf::Cursor cursor;

            if (!isHovered_) {
                isHovered_ = true;
                if(cursor.loadFromSystem(sf::Cursor::Hand))
                    window.setMouseCursor(cursor);
                return onHover(); // Вызов функции при наведении
            }
            // Проверка, нажата ли кнопка
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                isClicked_ = true;
                return preClick();
            } else if (event.type == sf::Event::MouseButtonReleased &&
                event.mouseButton.button == sf::Mouse::Left && isClicked_) {
                isClicked_ = false;
                return onClick();  // Вызов функции при отпускании
            }
            
        } else {
            if (isHovered_) {
                sf::Cursor cursor;
                isHovered_ = false;
                isClicked_ = false;
                if(cursor.loadFromSystem(sf::Cursor::Arrow))
                    window.setMouseCursor(cursor);
                return onLeave(); // Вызов функции при покидании курсора
            }
        }
        return sf::Event::Closed;
    }

    // Изменение текстуры спрайта
    void setTexture(const sf::Texture& texture) {
        sprite_.setTexture(texture);
    }

    void setTextureWithConst(const sf::Texture& texture) const {
        const_cast<Button*>(this)->setTexture(texture);
    }

    void setSprite(sf::Sprite&& sprite){
        sprite_ = std::move(sprite);
    }

    // Установка позиции кнопки
    void setPosition(const sf::Vector2f& position) {
        sprite_.setPosition(position);
        text_.setPosition(position);
    }

    void setPosition(float x, float y) {
        sprite_.setPosition(x, y);
        text_.setPosition(x, y);
    }
    
    //Получить позицию кнопки
    sf::Vector2f getPosition() const {
        return sprite_.getPosition();
    }

    //Получить размеры кнопки
    sf::FloatRect getBounds() const {
        return sprite_.getGlobalBounds();
    }

    void setText(sf::Text&& text) {
        text_ = std::move(text);
        constructorOrigin();
    }

    std::string getString() const {
        return text_.getString();
    }

    void setStyle(sf::Uint32 style) {
        text_.setStyle(style);
    }

    void setOrigin(float x, float y) {
        const sf::Vector2f& vect1 = sprite_.getOrigin();
        const sf::Vector2f& vect2 = text_.getOrigin();
        sprite_.setOrigin(vect1.x + x, vect1.y + y);
        text_.setOrigin(vect2.x + x, vect2.y + y);
    }

    // Вызов функции обратного вызова при клике
    sf::Event::EventType onClick() {
        if (onClick_) {
            onClick_(*this);
            return sf::Event::MouseButtonReleased;
        }
        return sf::Event::Closed;
    }

    sf::Event::EventType preClick() {
        if (preClick_) {
            preClick_(*this);
            return sf::Event::MouseButtonPressed;
        }
        return sf::Event::Closed;
    }

    // Вызов функции при наведении
    sf::Event::EventType onHover() {
        if (onHover_) {
            onHover_(*this);
            return sf::Event::MouseEntered;
        }
        return sf::Event::Closed;
    }

    // Вызов функции при покидании
    sf::Event::EventType onLeave() {
        if(onLeave_) {
            onLeave_(*this);
            return sf::Event::MouseLeft;
        }
        return sf::Event::Closed;
    }

private:
    sf::Text text_;

public:
    std::function<void(Button& this_)> onClick_;   // Функция обратного вызова при клике
    std::function<void(Button& this_)> preClick_ = {};
    std::function<void(Button& this_)> onLeave_ = {};   // Функция обратного вызова при покидании кнопки
    std::function<void(Button& this_)> onHover_ = {};   // Функция обратного вызова при наведении

private:
    bool isHovered_;           // Флаг, что курсор наведён
    bool isClicked_;           // Флаг, что кнопка нажата

    void constructorOrigin() {
        const sf::FloatRect& vect1 = sprite_.getGlobalBounds();
        const sf::FloatRect& vect2 = text_.getGlobalBounds();
        sprite_.setOrigin(vect1.width / 2, vect1.height / 2);
        text_.setOrigin(vect2.width / 2, text_.getCharacterSize() / 2);
    }
};

}

#endif