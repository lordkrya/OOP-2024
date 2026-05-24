#ifndef INPUTFIELD_DEFENDER
#define INPUTFIELD_DEFENDER

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <algorithm>
#include <limits>

#include "interfaceElem.hpp"

namespace simulation {

class InputField: public InterfaceElem {
public:
    
    InputField(const sf::Sprite& backGround, const sf::Text& text): 
        InterfaceElem(backGround), text_(text),
        cursorPosition_(0), selectionStart_(maxSelection), selectionEnd_(maxSelection), active_(0) {
        constructorOrigin();
        updateVisuals();
    }

    InputField(sf::Sprite&& backGround, sf::Text&& text): 
        InterfaceElem(std::move(backGround)), text_(std::move(text)),
        cursorPosition_(0), selectionStart_(maxSelection), selectionEnd_(maxSelection), active_(0) {
        constructorOrigin();
        updateVisuals();
    }

    void setPosition(float x, float y) {
        sprite_.setPosition(x, y);
        text_.setPosition(x, y);
        updateVisuals();
    }

    void setOrigin(float x, float y) {
        const sf::Vector2f& vect1 = sprite_.getOrigin();
        const sf::Vector2f vect2 = text_.getOrigin();
        sprite_.setOrigin(vect1.x + x, vect1.y + y);
        text_.setOrigin(vect2.x + x, vect2.y + y);
        cursor_.setOrigin(0, vect2.y + y);
        select_.setOrigin(0, - vect2.y - padding + y);
    }

    // Обработка событий ввода с клавиатуры
    sf::Event::EventType handleEvent(const sf::Event& event, sf::RenderWindow& window) {
        sf::FloatRect bounds = sprite_.getGlobalBounds();
        sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            if (bounds.contains(mousePosition)) {
                active_ = true;
                updateVisuals();
            } else {
                active_ = false;
            }
        }

        if (active_) {
            if (event.type == sf::Event::TextEntered) {
                if (isCharValid(event.text.unicode)) {
                    handleTextEntered(event.text.unicode);
                }
            } else if (event.type == sf::Event::KeyPressed) {
                handleKeyPressed(event.key.code);
            }
            return event.type;
        }
        return sf::Event::Closed;
    }

    // Отрисовка поля для ввода
    void draw(sf::RenderWindow& window) const {
        window.draw(sprite_);
        window.draw(text_);

        // Рисуем курсор только если нету выделения
        if (active_) {
            if(selectionStart_ == maxSelection) {
                window.draw(cursor_);
            } else {            
                window.draw(select_);
            }
        }
    }

    // Получение введенного текста
    const sf::String& getText() const {
        return text_.getString();
    }

    void setText(sf::String&& str) {
        return text_.setString(std::move(str));
    }

    std::string getString() const {
        return text_.getString();
    }


private:

    void constructorOrigin()  {
        sprite_.setOrigin(sprite_.getGlobalBounds().width / 2, sprite_.getGlobalBounds().height / 2);
        text_.setOrigin(-padding + sprite_.getGlobalBounds().width / 2, text_.getCharacterSize() / 2);
        cursor_.setOrigin(0, text_.getCharacterSize() / 2);
        select_.setOrigin(0, - padding - text_.getCharacterSize() / 2);
        
        text_.setPosition(sprite_.getPosition());

        cursor_.setSize(sf::Vector2f(2.0f, static_cast<float>(text_.getCharacterSize())));
        cursor_.setFillColor(text_.getFillColor());

        select_.setFillColor(sf::Color::Blue);
    }

    // Проверка символа
    bool isCharValid(unsigned int character) {
        if (character >= 32 && character <= 126)
            return true;
        return false;
    }

    // Обработка ввода текста
    void handleTextEntered(unsigned int unicode) {
        if (selectionStart_ != maxSelection) {
            deleteSelection();
        }
        
        sf::String newString = text_.getString();
        newString.insert(cursorPosition_, static_cast<char>(unicode));
        text_.setString(std::move(newString));
       
        cursorPosition_++;
        updateVisuals();
    }


    // Обработка нажатий клавиш
    void handleKeyPressed(sf::Keyboard::Key key) {
        switch (key) {
            case sf::Keyboard::Left:
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ||
                    sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)) {

                    if (selectionStart_ == maxSelection)
                        selectionStart_ = cursorPosition_;
                    if (cursorPosition_ > 0)
                        cursorPosition_--;
                    selectionEnd_ = cursorPosition_;

                } else {
                    if (cursorPosition_ > 0)
                        cursorPosition_--;
                    selectionStart_ = -1;
                    selectionEnd_ = -1;
                }
                break;

            case sf::Keyboard::Right:
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ||
                    sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)) {

                    if (selectionStart_ == maxSelection)
                        selectionStart_ = cursorPosition_;
                    if (cursorPosition_ < text_.getString().getSize())
                        cursorPosition_++;
                    selectionEnd_ = cursorPosition_;

                } else {
                    if (cursorPosition_ < text_.getString().getSize())
                        cursorPosition_++;
                    selectionStart_ = -1;
                    selectionEnd_ = -1;
                }
                break;

            case sf::Keyboard::BackSpace:
                if (selectionStart_ != maxSelection) {
                    deleteSelection();
                } else if (cursorPosition_ > 0) {
                    sf::String newString = text_.getString();
                    newString.erase(cursorPosition_ - 1);
                    text_.setString(std::move(newString));
                    cursorPosition_--;
                }
                break;

                case sf::Keyboard::Delete:
                    if (selectionStart_ != maxSelection) {
                        deleteSelection();
                    } else if (cursorPosition_ < text_.getString().getSize()) {
                        sf::String newString = text_.getString();
                        newString.erase(cursorPosition_);
                        text_.setString(std::move(newString));
                    }
                break;

            case sf::Keyboard::V:
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
                    sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) {

                    std::string clipboardText = sf::Clipboard::getString();
                    if (!clipboardText.empty()) {
                        if (selectionStart_ != maxSelection) {
                            deleteSelection();
                        }
                        
                        sf::String newString = text_.getString();
                        newString.insert(cursorPosition_, clipboardText);
                        text_.setString(newString);
                        cursorPosition_ += clipboardText.size();
                    }
                }
                break;

            case sf::Keyboard::A:
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) {
                    selectionStart_ = 0;
                    selectionEnd_ = text_.getString().getSize();
                    cursorPosition_ = selectionEnd_;
                }
                break;

            default:
                break;
        }
        updateVisuals();
    }

     // Удаление выделенного текста
     void deleteSelection() {
        if (selectionStart_ != maxSelection && selectionEnd_ != maxSelection && selectionStart_ != selectionEnd_) {
            int start = std::min(selectionStart_, selectionEnd_);
            int end = std::max(selectionStart_, selectionEnd_);
        
            sf::String newString = text_.getString();
            newString.erase(start, end - start);
            text_.setString(newString);
            cursorPosition_ = start;
            selectionStart_ = -1;
            selectionEnd_ = -1;
        }
    }

    void updateVisuals()
    {
        if (selectionStart_ == maxSelection) {
            sf::Vector2f textPos = text_.getPosition();
            float cursorX = text_.findCharacterPos(cursorPosition_).x;
            float cursorY = textPos.y;
            cursor_.setPosition(cursorX, cursorY);
        } else {
            float cursorX1 = text_.findCharacterPos(selectionStart_).x;
            float cursorX2 = text_.findCharacterPos(selectionEnd_).x;
            float cursorY = text_.getPosition().y;
            select_.setSize(sf::Vector2f(cursorX2 - cursorX1, 2.0f));
            select_.setPosition(cursorX1, cursorY);
        }
    }

private:
    sf::Text text_;
    sf::RectangleShape cursor_;
    size_t cursorPosition_;

    sf::RectangleShape select_;
    size_t selectionStart_;
    size_t selectionEnd_;

    bool active_;

    constexpr static float padding          = 2;
    constexpr static size_t maxSelection    = std::numeric_limits<size_t>::max();
};

}

#endif
