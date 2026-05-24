#ifndef INTERFACEBAR_DEFENDER
#define INTERFACEBAR_DEFENDER

#include <iostream>

#include "interfaceElem.hpp"
#include "memory"

namespace simulation {

class InterfaceBar: public InterfaceElem {
public:    
    InterfaceBar() = default;

    InterfaceBar(const sf::Sprite& sprite, bool active = 1): InterfaceElem(sprite), active_(active) {
        sprite_.setOrigin(sprite_.getGlobalBounds().width / 2, sprite_.getGlobalBounds().height / 2);
    }
    InterfaceBar(sf::Sprite&& sprite, bool active = 1): InterfaceElem(std::move(sprite)), active_(active) {
        sprite_.setOrigin(sprite_.getGlobalBounds().width / 2, sprite_.getGlobalBounds().height / 2);
    }

    InterfaceBar& operator = (InterfaceBar&& other) {
        swap(*this, other);
        return *this;
    }

    // Добавление элемента в меню-бар
    
    void addElement(std::unique_ptr<InterfaceElem>&& element, float x = 0, float y = 0) {
        setOriginElem(element);
        sf::FloatRect rect = sprite_.getGlobalBounds();
        (*element).setPosition(rect.width * (x / 2), rect.height * (y / 2));
        bar_.push_back(std::move(element));
    }

    // Вставка элемента в определенную позицию
    void insertElement(size_t index, std::unique_ptr<InterfaceElem>&& element) {
        setOriginElem(element);
        if (index <= bar_.size())
            bar_.insert(bar_.begin() + index, std::move(element));
    }
    
    // Удаление элемента по индексу
    void removeElement(size_t index) {
        if (index < bar_.size())
            bar_.erase(bar_.begin() + index);
    }
    
    // Удаление элемента по указателю
    void removeElement(const InterfaceElem* element) {
        auto it = std::find_if(bar_.begin(), bar_.end(),
            [element](const std::unique_ptr<InterfaceElem>& ptr) { return ptr.get() == element; }
        );
        
        if (it != bar_.end()) {
            bar_.erase(it);
        }
    }
    
    // Очистка всех элементов
    void clearElements() {
        bar_.clear();
    }
    
    // Получение элемента по индексу (константная версия)
    std::vector<std::unique_ptr<InterfaceElem>>::const_iterator getElement(size_t index) const {
        if (index < bar_.size())
            return bar_.begin() + index;
        return bar_.end();
    }
    
    // Получение элемента по индексу (неконстантная версия)
    std::vector<std::unique_ptr<InterfaceElem>>::iterator getElement(size_t index){
        if (index < bar_.size())
            return bar_.begin() + index;
        return bar_.end();
    }

    std::vector<std::unique_ptr<InterfaceElem>>::iterator getIter(const InterfaceElem* element){
        auto it = std::find_if(bar_.begin(), bar_.end(),
            [element](const std::unique_ptr<InterfaceElem>& ptr) { return ptr.get() == element; }
        );
        return it;
    }


    // Обновление и отрисовка меню-бара
    sf::Event::EventType handleEvent(const sf::Event& event, sf::RenderWindow& window) override { 
        if (active_) {       
            sf::FloatRect bounds = sprite_.getGlobalBounds();
            sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            if (bounds.contains(mousePosition)) {
                for(auto& element: bar_)
                    element->handleEvent(event, window);
                return sf::Event::MouseEntered;
            }
        }
        return sf::Event::Closed;
    }

    void draw(sf::RenderWindow& window) const override {
        if (active_) {
            window.draw(sprite_);
            for (const auto& element : bar_)
                element->draw(window);
        }
    }
    
    size_t getSize() const {
        return bar_.size();
    }

    void changeActive() {
        active_ = !active_;
    }

    friend void swap(InterfaceBar& first, InterfaceBar& second) noexcept;

    private:

        void setOriginElem(std::unique_ptr<InterfaceElem>& element) {
            const sf::Vector2f& vect = sprite_.getPosition();
            (*element).setOrigin(-vect.x, -vect.y);
        }

        std::vector<std::unique_ptr<InterfaceElem>> bar_;
        bool active_ = true;
};

}

#endif