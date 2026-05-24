#ifndef MAINSIMULATION_DEFENDER
#define MAINSIMULATION_DEFENDER

#include "simulation.hpp"

using namespace training;

namespace simulation {

class MainSimulation {
private:
    sf::RenderWindow playWindow;
    InterfaceBar gameBar;

public:
    MainSimulation() = default;

    static void mainS() {
        MainSimulation sim;
        sim.GamеStart();
    }

    void GamеStart() {
        // Создаём окно
        playWindow.close();
        playWindow.create(sf::VideoMode::getDesktopMode(), "SoC - Simulation", sf::Style::Fullscreen);
        playWindow.setFramerateLimit(60);

        float width = sf::VideoMode::getDesktopMode().width;
        float height = sf::VideoMode::getDesktopMode().height;

        // Получаем готовые текстуры и конфиги
        TexturePack& textures = TexturePack::getInstance();

        Configs& configs = Configs::getInstance();
        json mainConfig = configs.getMainConfig();
        configs.loadMapConfig(mainConfig["map"]);
        configs.loadFirstTeamConfig(mainConfig["firstTeam"]);
        configs.loadSecondTeamConfig(mainConfig["secondTeam"]);


        // Считываем конфиги
        Training data;
        data.setFirstGroup(configs.readFirstDivision());
        data.setSecondGroup(configs.readSecondDivision());
        data.setMap(configs.readMap());
        data.synchronization();
        bool playData = false;

        // Создаём фон
        sf::RectangleShape background_play(sf::Vector2f(width, height));
        background_play.setTexture(textures.getTexture("backGroundSimulation.jpg"));

        // Создаём карту и класс для рендера группы армий
        float diagonal = std::min(width, height);
        MatrixMap tempMap = MatrixMap(sf::Vector2f(diagonal*0.95, diagonal*0.95), 
            textures.getTexture("map.png"), sf::Vector2f(width / 3, height / 2));

        ArmyGroup renderArmy = ArmyGroup(std::move(tempMap), data.getMap(), {100, 50},
            textures.getTexture("helmet1.png"),
            textures.getTexture("helmet2.png"),
            {102, 90}, textures.getTexture("battle.png")
        );

        // Создаём панель управления симуляцей
        sf::Sprite backBar(*textures.getTexture("gameBar.png"), sf::IntRect(0, 0, 700, 175));
        backBar.setPosition(width - 350, 87);
        gameBar = InterfaceBar(std::move(backBar));


        // Задаём параметры кнопок из бокового меню
        sf::Sprite buttonEnd(*textures.getTexture("menuButton.png"), sf::IntRect(0, 0, 175, 125));
        sf::Sprite buttonStop(*textures.getTexture("menuButton.png"), sf::IntRect(0, 0, 175, 125));
        sf::Sprite buttonPlay(*textures.getTexture("menuButton.png"), sf::IntRect(0, 0, 175, 125));
        auto barFuncClick = [](Button& this_) {
            const TexturePack& textures = TexturePack::getInstance();
            this_.setTextureWithConst(*textures.getTexture("menuButton.png"));
        };
        auto barFuncPreClick = [](Button& this_) {
            const TexturePack& textures = TexturePack::getInstance();
            this_.setTextureWithConst(*textures.getTexture("menuChooseButton.png"));
        };

        auto funcButtonEnd = [&](Button& this_) {
            playWindow.close();
        };
        auto funcButtonPlay = [&](Button& this_) {
            barFuncClick(this_);
            playData = true;
        };
        auto funcButtonStop = [&](Button& this_) {
            barFuncClick(this_);
            playData = false;
        };

        gameBar.addElement(std::make_unique<Button>(std::move(buttonEnd), 
            funcButtonEnd, barFuncPreClick, barFuncClick), 0.7, -0.1);
        gameBar.addElement(std::make_unique<Button>(std::move(buttonPlay),
            funcButtonPlay, barFuncPreClick, barFuncClick), 0.1, -0.1);
        gameBar.addElement(std::make_unique<Button>(std::move(buttonStop),
            funcButtonStop, barFuncPreClick, barFuncClick), -0.5, -0.1);


        while (playWindow.isOpen()) {
            sf::Event eventPlay;
            while (playWindow.pollEvent(eventPlay)) {
                if (eventPlay.type == sf::Event::KeyPressed) {
                    if (eventPlay.key.code == sf::Keyboard::Escape) { playWindow.close(); }
                }

                gameBar.handleEvent(eventPlay, playWindow);
                renderArmy.handleEvent(eventPlay, playWindow);
            }
    
            if (playData)
                data.doTick();
            
            playWindow.clear();
            playWindow.draw(background_play);

            renderArmy.draw(playWindow);
            gameBar.draw(playWindow);

            playWindow.display();
        }
    }

};

}

#endif
