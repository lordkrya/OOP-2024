#include "texturePack.hpp"

namespace simulation {

TexturePack* TexturePack::instance = nullptr;

bool TexturePack::addTexture(const std::string& filePath) {
    fs::path realPath(filePath);
    std::string name = realPath.filename().string();
    if (textures_.count(name) > 0) return false;
    
    sf::Texture texture;
    if (!texture.loadFromFile(filePath)) return false;
    
    auto result = textures_.insert({name, std::move(texture)});
    return result.second;
}

void TexturePack::loadFont(const std::string& filePath) {
    font_.loadFromFile(filePath);
}

void TexturePack::loadIcon(const std::string& filePath) {
    icon_.loadFromFile(filePath);
}

void TexturePack::loadTexturesFromDirectory(const std::string& directoryPath) {
    try {
        for (const auto& entry: fs::recursive_directory_iterator(directoryPath)) {
            if (entry.is_regular_file() && (entry.path().extension() == ".jpg" || entry.path().extension() == ".png"))
                addTexture(entry.path().string());
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception while loading textures: " << e.what() << std::endl;
    }
}

const sf::Texture* TexturePack::getTexture(const std::string& filePath) const {
    auto it = textures_.find(filePath);
    if (it != textures_.end()) {
        return &(it->second);
    } else {
        return nullptr;
    }
}

}