#ifndef MENU_DEFENDER
#define MENU_DEFENDER

#include <SFML/Graphics.hpp>
#include <vector>

#include "button.hpp"
#include "texturePack.hpp"

namespace simulation {

class GameMenu {
private:
    float menuX_;
    float menuY_;
    int menuStep_;
    size_t maxMenu_;
    size_t buttonSelected_ = 0;

    std::vector<Button> mainMenu_;
    
    const sf::Texture* buttonsTexture_;
    const sf::Texture* chooseButtonsTexture_;

    constexpr static int width_                 = 650;
    constexpr static int height_                = 125;
    constexpr static unsigned int characterSize = 64;
    constexpr static char menuButton[]          = "menuButton.png";
    constexpr static char menuChooseButton[]    = "menuChooseButton.png";

    sf::Sprite getSpriteButton();
    sf::Text getTextButton(std::string&& str, const TexturePack& textures);
public:
    GameMenu() = default;

    GameMenu(float menuX, float menuY, int menuStep, 
        std::vector<std::pair<std::function<void(Button &this_)>, std::string>>&& funcs);

    GameMenu(GameMenu&& other);

    GameMenu &operator = (GameMenu&& other);

    ~GameMenu() = default;

    void draw(sf::RenderWindow& window) const;

    void chooseButton(size_t buttonSelected);

    void moveUp();

    void moveDown();

    void press();

    bool handleEvent(const sf::Event& event, sf::RenderWindow& window);

    unsigned int getMaxMenu() { return maxMenu_; }

    unsigned int getButtonSelected() { return buttonSelected_; }

    friend void swap(GameMenu& first, GameMenu& second);
};

}

#endif