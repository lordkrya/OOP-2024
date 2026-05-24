#ifndef CONFIGS_DEFENDER
#define CONFIGS_DEFENDER

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

#include <stdio.h>

#include "nlohmann\json.hpp"
#include "division.hpp"
#include "map.hpp"
#include "configReader.hpp"

namespace simulation {

using json = nlohmann::json;

class Configs {
private:
    static Configs* instance;

    json mainConfig;
    std::string pathMainConfig;
    json mapConfig;
    std::string pathMapConfig;
    json firstTeamConfig;
    std::string pathFirstTeamConfig;
    json secondTeamConfig;
    std::string pathSecondTeamConfig;
    
    std::mutex configMutex;

public:
    static Configs& getInstance() {
        if (!instance) {
            instance = new Configs();
        }
        return *instance;
    }
    
    bool loadMainConfig(const std::string& filePath) {
        std::lock_guard<std::mutex> lock(configMutex);
        return loadConfig(filePath, mainConfig, pathMainConfig);
    }

    bool loadMapConfig(const std::string& filePath) {
        std::lock_guard<std::mutex> lock(configMutex);
        return loadConfig(filePath, mapConfig, pathMapConfig);
    }

    bool loadFirstTeamConfig(const std::string& filePath) {
        std::lock_guard<std::mutex> lock(configMutex);
        return loadConfig(filePath, firstTeamConfig, pathFirstTeamConfig);
    }

    bool loadSecondTeamConfig(const std::string& filePath) {
        std::lock_guard<std::mutex> lock(configMutex);
        return loadConfig(filePath, secondTeamConfig, pathSecondTeamConfig);
    }

    // Метод сохранения конфигов
    bool saveAllConfigs() {
        std::lock_guard<std::mutex> lock(configMutex);
        return  saveConfig(pathMainConfig, mainConfig) &&
                saveConfig(pathMapConfig, mapConfig) &&
                saveConfig(pathFirstTeamConfig, firstTeamConfig) &&
                saveConfig(pathSecondTeamConfig, secondTeamConfig);
    }

    // Безопасное чтение без гонки данных
    team::Division readFirstDivision() {
        std::lock_guard<std::mutex> lock(configMutex);
        return firstTeamConfig.at("division").get<team::Division>();
    }

    team::Division readSecondDivision() {
        std::lock_guard<std::mutex> lock(configMutex);
        return secondTeamConfig.at("division").get<team::Division>();
    }

    map::Map readMap() {
        std::lock_guard<std::mutex> lock(configMutex);
        return mapConfig.at("map").get<map::Map>();
    }

    // Геттеры для получения копии json (ссылка невозможно, т.к. будет рассинхрон)
    json getMainConfig() {
        std::lock_guard<std::mutex> lock(configMutex);
        return mainConfig;
    }

    json getMapConfig() {
        std::lock_guard<std::mutex> lock(configMutex);
        return mapConfig;
    }

    json getFirstTeamConfig() {
        std::lock_guard<std::mutex> lock(configMutex);
        return firstTeamConfig;
    }

    json getSecondTeamConfig() {
        std::lock_guard<std::mutex> lock(configMutex);
        return secondTeamConfig;
    }

    // Методы для изменения определенных полей
    bool setMainConfigValue(const std::string& key, const json& value) {
        std::lock_guard<std::mutex> lock(configMutex);
        try {
            mainConfig[key] = value;
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Error: Could not set " << key << " with "
            << value << " for main config. " << e.what() << std::endl;
            return false;
        }
    }

    bool setMapConfigValue(const std::string& key, const json& value) {
        std::lock_guard<std::mutex> lock(configMutex);
        try {
            mapConfig[key] = value;
            return true;
        } catch (const std::exception& e) {
           std::cerr << "Error: Could not set " << key << " with "
           << value << " for map config. " << e.what() << std::endl;
           return false;
        }
    }

    bool setFirstTeamConfigValue(const std::string& key, const json& value) {
        std::lock_guard<std::mutex> lock(configMutex);
        try {
           firstTeamConfig[key] = value;
           return true;
         } catch (const std::exception& e) {
            std::cerr << "Error: Could not set " << key << " with "
            << value << " for first team config. " << e.what() << std::endl;
            return false;
        }
    }

    bool setSecondTeamConfigValue(const std::string& key, const json& value) {
        std::lock_guard<std::mutex> lock(configMutex);
        try {
            secondTeamConfig[key] = value;
            return true;
        } catch (const std::exception& e) {
           std::cerr << "Error: Could not set " << key << " with "
           << value << " for second team config. " << e.what() << std::endl;
           return false;
        }
    }


private:
    // Вспомогательные методы для загрузки и сохранения
    static bool loadConfig(const std::string& filePath, json& config, std::string& pathConfig) {
        try {
            std::ifstream inputFile(filePath);
            if (!inputFile.is_open()) {
                std::cerr << "Error: Could not open file " << filePath << std::endl;
                return false;
            }
            inputFile >> config;
            pathConfig = filePath;
            inputFile.close();

            return true;
        } catch (const json::parse_error& e) {
            std::cerr << "Error parsing JSON from " << filePath << ": " << e.what() << std::endl;
            return false;
        } catch (const std::exception& e) {
            std::cerr << "Error loading file " << filePath << ": " << e.what() << std::endl;
            return false;
        }
    }

    static bool saveConfig(const std::string& filePath, const json& config) {
        try {
            std::ofstream outputFile(filePath);
            if (!outputFile.is_open()) {
                std::cerr << "Error: Could not open file " << filePath << " for writing." << std::endl;
                return false;
            }
            outputFile << std::setw(4) << config << std::endl;
            outputFile.close();
            return true;
        } catch(const std::exception& e) {
            std::cerr << "Error saving file " << filePath << ": " << e.what() << std::endl;
            return false;
        }
    }

};

}

#endif