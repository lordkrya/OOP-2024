#ifndef TEXTUREPACK_DEFENDER
#define TEXTUREPACK_DEFENDER

#include <iostream>
#include <string>
#include <filesystem>
#include <unordered_map>
#include <SFML/Graphics.hpp>

namespace fs = std::filesystem;

namespace simulation {

class TexturePack {
private:
    static TexturePack* instance;

    std::unordered_map<std::string, sf::Texture> textures_;
    sf::Font font_;
    sf::Image icon_;

    TexturePack() {}
    TexturePack(const TexturePack&) = delete;
    TexturePack& operator=(const TexturePack&) = delete;

public:
    static TexturePack& getInstance() {
        if (!instance) {
            instance = new TexturePack();
        }
        return *instance;
    }

    // Удаление singleton (при завершении работы)
    static void destroyInstance() { delete instance; instance = nullptr; }
    
    // Добавляет текстуру, возвращает true при успехе, false при неудаче
    bool addTexture(const std::string& filePath);

    void loadFont(const std::string& filePath);

    void loadIcon(const std::string& filePath);

    // Загружает все текстуры из указанной папки (рекурсивно)
    void loadTexturesFromDirectory(const std::string& directoryPath);

    const sf::Font& getFont() const { return font_; }

    const sf::Image& getIcon() const { return icon_; }

    // Возвращает текстуру по пути.  Возвращает nullptr, если текстура не найдена.
    const sf::Texture* getTexture(const std::string& filePath) const;

    // Другие полезные функции, например:
    size_t getTextureCount() const { return textures_.size(); }

    //Очистка текстур
    void clearTextures() { textures_.clear(); }
};

}

#endif