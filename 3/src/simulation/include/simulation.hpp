#ifndef SIMULATION_DEFENDER
#define SIMULATION_DEFENDER

#include <iostream>
#include <SFML/Graphics.hpp>
#include <thread>

#include <windows.h>
#include <commdlg.h>

#include "menu.hpp"
#include "button.hpp"
#include "inputField.hpp"
#include "interfaceBar.hpp"
#include "configs.hpp"
#include "configReader.hpp"
#include "matrixMap.hpp"
#include "training.hpp"
#include "armyGroup.hpp"
#include "mainSimulation.hpp"

using namespace training;

namespace simulation {

class Simulation {
private:
    sf::RenderWindow window;
    InterfaceBar menuBar;
    GameMenu myMenu;

    static void InitText(sf::Text& mText, float xPos, float yPos, 
        sf::Color textColor = sf::Color::White, int bord = 0, sf::Color borderColor = sf::Color::Black) {
        mText.setFillColor(textColor);
        mText.setOutlineThickness(bord);
        mText.setOutlineColor(borderColor);

        sf::FloatRect rect = mText.getGlobalBounds();
        mText.setOrigin(rect.width / 2, rect.height / 2);
        mText.setPosition(xPos, yPos);
    }

    void openFileDialog(Button &this_) {
        OPENFILENAMEA ofn;
        char szFile[260] = { 0 };
        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = NULL;
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = "All Files (*.*)\0*.*\0";
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        if (GetOpenFileNameA(&ofn) == TRUE) {
            auto it = menuBar.getIter(&this_);
            (**++it).setText(sf::String(szFile));
        }
    }

public:

    Simulation() = default;

    void createGameThread(Button &this_) {
        std::thread th(MainSimulation::mainS);
        th.detach();
    }

    int startMenu() {

        // Создаём окно
        window.create(sf::VideoMode::getDesktopMode(), "SoC - Menu"); //, Style::Fullscreen);
        window.setFramerateLimit(60);

        float width = sf::VideoMode::getDesktopMode().width;
        float height = sf::VideoMode::getDesktopMode().height;


        // Подкачиваем текстурпак
        TexturePack& textures = TexturePack::getInstance();
        textures.loadTexturesFromDirectory("textures/");
        textures.loadFont("fonts/GULAG Pavljenko.otf");
        textures.loadIcon("other/icon.png");

        // Устанавливаем иконку
        window.setIcon(512, 512, textures.getIcon().getPixelsPtr());

        // Подкачиваем конфиги
        Configs& configs = Configs::getInstance();
        configs.loadMainConfig("configs/config.json");


        // Задний фон и название
        sf::RectangleShape backGround(sf::Vector2f(width, height));
        backGround.setTexture(textures.getTexture("backGround.png"));


        sf::Text Titul("Stomach of\n   CHUGUN", textures.getFont(), 100);
        Titul.setStyle(sf::Text::Bold);
        InitText(Titul, width / 3, height / 2.35, sf::Color(255, 255, 255), 3);


        // Создаём кнопоки основного меню
        std::vector<std::pair<std::function<void(Button &this_)>, std::string>> funcs
            // Связываем метод с экземпляром данного меню (т.к. метод не просто функция)
            {{std::bind(&createGameThread, this, std::placeholders::_1), "Start"},
            {[&](Button &this_){ menuBar.changeActive(); }, "Settings"},
            {{}, "Details"},
            {[](Button &this_){}, "Exit"}};
        myMenu = GameMenu(width / 3, height / 1.55, 150, std::move(funcs));


        // Задаём параметры поля для ввода
        sf::Text buf("", textures.getFont(), 20);
        buf.setFillColor(sf::Color::Black);
        sf::Sprite rec(*textures.getTexture("inputField.png"), sf::IntRect(0, 0, 550, 30));


        // Задаём параметры кнопок из бокового меню
        sf::Sprite buttonBarSprite(*textures.getTexture("menuButton.png"), sf::IntRect(0, 0, 400, 100));
        sf::Text buttonBarText("Simulation", textures.getFont(), 40);
        sf::Sprite buttonSaveSprite(*textures.getTexture("menuButton.png"), sf::IntRect(0, 0, 300, 75));
        sf::Text buttonSaveText("Save", textures.getFont(), 30);
        sf::Sprite buttonChooseSprite1(*textures.getTexture("menuButton.png"), sf::IntRect(0, 0, 550, 60));
        sf::Text buttonChooseText1("Select the file 1 or write the path", textures.getFont(), 25);
        sf::Sprite buttonChooseSprite2(*textures.getTexture("menuButton.png"), sf::IntRect(0, 0, 550, 60));
        sf::Text buttonChooseText2("Select the file 2 or write the path", textures.getFont(), 25);
        auto barFuncClick = [](Button& this_) {
            const TexturePack& textures = TexturePack::getInstance();
            this_.setTextureWithConst(*textures.getTexture("menuButton.png"));
        };
        auto barFuncPreClick = [](Button& this_) {
            const TexturePack& textures = TexturePack::getInstance();
            this_.setTextureWithConst(*textures.getTexture("menuChooseButton.png"));
        };


        // Создаём боковое меню
        sf::Sprite backBar(*textures.getTexture("backMenuBar.png"), sf::IntRect(0, 0, 800, 1200));
        backBar.setPosition(width * 0.825, height * 0.5);
        menuBar = InterfaceBar(std::move(backBar), false);

        std::unique_ptr<Button> buttonBar = std::make_unique<Button>(
            std::move(buttonBarSprite), barFuncClick, barFuncPreClick, barFuncClick);
        (*buttonBar).setText(std::move(buttonBarText));
        menuBar.addElement(std::move(buttonBar), 0, -0.6);

        std::unique_ptr<Button> buttonSave = std::make_unique<Button>(
            std::move(buttonSaveSprite),
            [&] (Button &this_) {
                auto it = menuBar.getIter(&this_);
                configs.setMainConfigValue("firstTeam", (**(it + 2)).getString());
                configs.setMainConfigValue("secondTeam", (**(it + 4)).getString());
                barFuncClick(this_);
            },
            barFuncPreClick,
            barFuncClick
        );

        (*buttonSave).setText(std::move(buttonSaveText));
        menuBar.addElement(std::move(buttonSave), 0, 0.6);

        std::unique_ptr<Button> buttonChoose1 = std::make_unique<Button>(
            std::move(buttonChooseSprite1), std::bind(&openFileDialog, this, std::placeholders::_1));
        (*buttonChoose1).setText(std::move(buttonChooseText1));
        menuBar.addElement(std::move(buttonChoose1), 0, -0.25);

        menuBar.addElement(std::make_unique<InputField>(rec, buf), 0, -0.15);

        std::unique_ptr<Button> buttonChoose2 = std::make_unique<Button>(
            std::move(buttonChooseSprite2), std::bind(&openFileDialog, this, std::placeholders::_1));
        (*buttonChoose2).setText(std::move(buttonChooseText2));
        menuBar.addElement(std::move(buttonChoose2), 0, 0.15);

        menuBar.addElement(std::make_unique<InputField>(rec, buf), 0, 0.25);


        // И наконец запуск
        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if(event.type == sf::Event::Closed) window.close();

                // Необходимо отдельно обрабатывать событие нажатий на клавиши
                // Т.к. button само по себе работает лишь с мышью
                if (event.type == sf::Event::KeyReleased) {
                    if (event.key.code == sf::Keyboard::Up) myMenu.moveUp();
                    if (event.key.code == sf::Keyboard::Down) myMenu.moveDown();
                    if (event.key.code == sf::Keyboard::Return) {
                        if (myMenu.getButtonSelected() + 1 == myMenu.getMaxMenu()) window.close();
                        myMenu.press();
                    }
                }

                menuBar.handleEvent(event, window);

                if (myMenu.handleEvent(event, window)) 
                    window.close();
            }

            window.clear();
            window.draw(backGround);
            window.draw(Titul);

            myMenu.draw(window);
            menuBar.draw(window);

            window.display();
        }
        TexturePack::destroyInstance();
        return 0;
    }
};

}

#endif
