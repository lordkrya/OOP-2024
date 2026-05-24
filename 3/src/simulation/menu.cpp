#include "menu.hpp"

namespace simulation {

sf::Sprite GameMenu::getSpriteButton() {
	return sf::Sprite(*buttonsTexture_, sf::IntRect(0, 0, width_, height_));
}

sf::Text GameMenu::getTextButton(std::string&& str, const TexturePack& textures) {
	return sf::Text(std::move(str), textures.getFont(), characterSize);
}

GameMenu::GameMenu(float menuX, float menuY, int menuStep,
	std::vector<std::pair<std::function<void(Button &this_)>, std::string>>&& funcs):
	menuX_(menuX), menuY_(menuY), menuStep_(menuStep), maxMenu_(funcs.size()) {
	
	const TexturePack& textures = TexturePack::getInstance();
	buttonsTexture_ = textures.getTexture(menuButton);
    chooseButtonsTexture_ = textures.getTexture(menuChooseButton);
	if (!buttonsTexture_ || !chooseButtonsTexture_) throw std::logic_error("");

    mainMenu_.reserve(maxMenu_);
	for (size_t i = 0, yPos = menuY_; i < maxMenu_; i++, yPos += menuStep_) {
		Button button(getSpriteButton(), funcs[i].first);
		button.setText(getTextButton(std::move(funcs[i].second), textures));
		button.onHover_ = [](Button &this_){};
		button.setPosition(menuX_, yPos);
		mainMenu_.push_back(std::move(button));
	}
	mainMenu_[0].setTexture(*chooseButtonsTexture_);
}

GameMenu::GameMenu(GameMenu&& other):
	menuX_(other.menuX_),
	menuY_(other.menuY_),
	menuStep_(other.menuStep_),
	maxMenu_(other.maxMenu_),
	buttonSelected_(other.buttonSelected_),
	mainMenu_(std::move(other.mainMenu_)),
	buttonsTexture_(other.buttonsTexture_),
	chooseButtonsTexture_(other.chooseButtonsTexture_) 
{}

void swap(GameMenu& first, GameMenu& second) {
	std::swap(first.menuX_, second.menuX_);
	std::swap(first.menuY_, second.menuY_);
	std::swap(first.menuStep_, second.menuStep_);
	std::swap(first.maxMenu_, second.maxMenu_);
	std::swap(first.buttonSelected_, second.buttonSelected_);
	std::swap(first.mainMenu_, second.mainMenu_);
	std::swap(first.buttonsTexture_, second.buttonsTexture_);
	std::swap(first.chooseButtonsTexture_, second.chooseButtonsTexture_);
}

GameMenu &GameMenu::operator = (GameMenu&& other) {
	swap(*this, other);
	return *this;
}

void GameMenu::chooseButton(size_t buttonSelected) {
	if (buttonSelected < maxMenu_) {
		mainMenu_[buttonSelected_].setTexture(*buttonsTexture_);
		mainMenu_[buttonSelected_].setStyle(sf::Text::Regular);
		mainMenu_[buttonSelected].setTexture(*chooseButtonsTexture_);
		mainMenu_[buttonSelected].setStyle(sf::Text::Bold);
		buttonSelected_ = buttonSelected;
	}
}

void GameMenu::moveUp() {
	if (buttonSelected_ > 0) {
		chooseButton(buttonSelected_ - 1);
	} else {
		chooseButton(maxMenu_ - 1);
	}
}

void GameMenu::moveDown() {
	if (buttonSelected_ + 1 < maxMenu_) {
		chooseButton(buttonSelected_ + 1);
	} else {
		chooseButton(0);
	}
}

bool GameMenu::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
	for (size_t i = 0; i < maxMenu_; i++) {
		sf::Event::EventType type = mainMenu_[i].handleEvent(event, window);

		if (type == sf::Event::MouseButtonReleased && i + 1 == maxMenu_)
			return true;

		if (type == sf::Event::MouseEntered)
			chooseButton(i);
	}
	return false;
}

void GameMenu::press() {
	mainMenu_[buttonSelected_].onClick();
}

void GameMenu::draw(sf::RenderWindow& window) const {
    for (size_t i = 0; i < maxMenu_; i++) mainMenu_[i].draw(window);
}

}