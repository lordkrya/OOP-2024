#ifndef CONFIGREADER_DEFENDER
#define CONFIGREADER_DEFENDER

#include "training.hpp"
#include "soldier.hpp"
#include "map.hpp"

using namespace nlohmann;
using namespace team;
using namespace soldiers;
using namespace map;

typedef table::OrderedTable<unsigned int, SoldierStatus> table_sold;
typedef table::Elem<unsigned int, SoldierStatus> elem_sold;

namespace nlohmann {

template<>
struct adl_serializer<Location> {
    static void from_json(const json& cfg, Location& p) {
        cfg.at("x").get_to(p.x_);
        cfg.at("y").get_to(p.y_);
    }
};

template<>
struct adl_serializer<Soldier> {
    static void from_json(const json& cfg, Soldier& s) {
        s.setName(cfg.at("name").get<std::string>());
        s.setRang(cfg.at("rank").get<Rang>());
        s.setUniform(cfg.at("uniform_size").get<unsigned int>());
        s.setSkills(cfg.at("skills").get<std::vector<Skill>>());
    }
};

template<>
struct adl_serializer<Battalion> {
    static void from_json(const json& cfg, Battalion& b) {
        static unsigned int N = 0;
        
        table_sold& table = b.getTable();

        SoldierStatus sld;
        for (auto it: cfg.at("soldiers").get<json>()) {
            adl_serializer<Soldier>::from_json(it, sld.person_);
            sld.status_ = efficient;
            
            table.emplace_hint(table.cend(), N, std::move(sld));
            N++;
        }

        b.setPosition(cfg.at("position").get<Location>());
    }
};

template<>
struct adl_serializer<std::vector<std::shared_ptr<Battalion>>> {
    static void from_json(const json& cfg, std::vector<std::shared_ptr<Battalion>>& bat) {
        bat.clear();
        for (const auto& item: cfg) {
            std::shared_ptr<Battalion> b = std::make_shared<Battalion>();
            adl_serializer<Battalion>::from_json(item, *b);
            bat.push_back(std::move(b));
        }
    }
};

template<>
struct adl_serializer<Division> {
    static void from_json(const json& cfg, Division& d) {
        d.setBattalions(cfg.at("battalions").get<std::vector<std::shared_ptr<Battalion>>>());
    }
};

template<>
struct adl_serializer<FieldAttribute> {
    static void from_json(const json& cfg, FieldAttribute& fa) {
        cfg.at(0).get_to(fa.ID_);
        cfg.at(1).get_to(fa.level_);
    }
};

template<>
struct adl_serializer<std::vector<FieldAttribute>> {
    static void from_json(const json& cfg, std::vector<FieldAttribute>& vec) {
        vec.clear();
        for (const auto& item : cfg) {
            if (!item.empty()) {
                FieldAttribute fa;
                adl_serializer<FieldAttribute>::from_json(item, fa);
                vec.push_back(fa);
            }
        }
    }
};

template<>
struct adl_serializer<Field> {
    static void from_json(const json& cfg, Field& f) {
        std::vector<FieldAttribute> vec;
        adl_serializer<std::vector<FieldAttribute>>::from_json(cfg.at("attributes"), vec);
        
        f.setRegion(cfg.at("region").get<Region>());
        f.setAttrubutes(std::move(vec));
    }
};

template<>
struct adl_serializer<Map> {
    static void from_json(const json& cfg, Map& m) {
        m.clear();
        m.setName(cfg.at("name").get<std::string>());
        m.setSizeX(cfg.at("sizeX").get<size_t>());
        m.setSizeY(cfg.at("sizeY").get<size_t>());
        m.resize();

        Field f;
        for (const auto& item: cfg.at("fields")) {
            adl_serializer<Field>::from_json(item, f);
            //size_t x = cfg.at("position").at(0);
            //size_t y = cfg.at("position").at(1);
            m(0, 0) = std::move(f);
        }
    }
};

}


namespace simulation {

class ConfigReader {
public:
    // Устаревшее. Вероятность гонки данных
    static Division readDivision(const json& cfg) {
        return cfg.at("division").get<Division>();
    }
    // Устаревшее. Вероятность гонки данных
    static Map readMap(const json& cfg) {
        return cfg.at("map").get<Map>();
    }
};

}

#endif
