#ifndef ARMYGROUP_DEFENDER
#define ARMYGROUP_DEFENDER

#include "matrixMap.hpp"
#include "battalion.hpp"
#include "map.hpp"

namespace simulation {

class ArmyGroup: public MatrixMap {
public:
    ArmyGroup(MatrixMap&& matrix, 
    map::Map& map, const sf::Vector2f& size,
    const sf::Texture* texture1, const sf::Texture* texture2,
    const sf::Vector2f& sizeBattle, const sf::Texture* battleTexture) noexcept:
        MatrixMap(std::move(matrix)), map_(map), helmet1_(size), helmet2_(size), 
        battle_(sizeBattle), mapBounds(rect_.getGlobalBounds()) {

        helmet1_.setOrigin(size.x / 2, size.y / 2);
        helmet1_.setTexture(texture1);
        helmet2_.setOrigin(size.x / 2, size.y / 2);
        helmet2_.setTexture(texture2);

        battle_.setOrigin(sizeBattle.x / 2, sizeBattle.y / 2);
        battle_.setTexture(battleTexture);

        concentrationX = mapBounds.width / map_.getSizeX() / map::Map::metersInSquare;
        concentrationY = mapBounds.height / map_.getSizeY() / map::Map::metersInSquare;
    }

    void draw(sf::RenderWindow& window) {
        window.draw(rect_);

        for (size_t x = 0; x < map_.getSizeX(); x++) {
            for (size_t y = 0; y < map_.getSizeY(); y++) {
                
                for (const auto& it: map_(x,y).getBattalionsFirst()) {
                    if (auto sharedBattalion = it.lock()) {
                        const team::Location& loc = sharedBattalion->getPosition();
                        helmet1_.setPosition(calculatePosition(loc));
                        window.draw(helmet1_);
                    }
                }

                for (const auto& it: map_(x,y).getBattalionsSecond()) {
                    if (auto sharedBattalion = it.lock()) {
                        const team::Location& loc = sharedBattalion->getPosition();
                        helmet2_.setPosition(calculatePosition(loc));
                        window.draw(helmet2_);
                    }
                }

                for (size_t x = 0; x < map_.getSizeX(); x++) {
                    for (size_t y = 0; y < map_.getSizeY(); y++) {
                        Field& field = map_(x, y);
                        if (!field.emptyFirst() && !field.emptySecond()) {
                            battle_.setPosition(calculatePosition(
                                x * map::Map::metersInSquare + map::Map::metersInSquare / 2, 
                                y * map::Map::metersInSquare + map::Map::metersInSquare / 2)
                            );
                            window.draw(battle_);
                        }
                    }
                }
            }
        }
    }

    sf::Vector2f calculatePosition(const team::Location& loc) {
        return {(concentrationX * loc.x_ - mapBounds.width / 2 + 20) * scale_ + position_.x,
                ( - concentrationY * loc.y_ + mapBounds.height / 2 - 20) * scale_ + position_.y};
    }

    sf::Vector2f calculatePosition(float x, float y) {
        return {(concentrationX * x - mapBounds.width / 2 + 20) * scale_ + position_.x,
                ( - concentrationY * y + mapBounds.height / 2 - 20) * scale_ + position_.y};
    }

private:
    map::Map& map_;
    
    sf::RectangleShape helmet1_;
    sf::RectangleShape helmet2_;

    sf::RectangleShape battle_;

    sf::FloatRect mapBounds;         // Размер карты в пикселях без разширения
    float concentrationX;           // Кол-во пикселей на игровой метр по X
    float concentrationY;           // Кол-во пикселей на игровой метр по Y
};

}

#endif
