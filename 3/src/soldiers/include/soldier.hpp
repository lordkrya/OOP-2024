#ifndef SOLDIER_DEFENDER
#define SOLDIER_DEFENDER

#include <vector>
#include <string>

#include "soldierStatus.hpp"
#include "specType.hpp"
#include "fighter.hpp"

namespace soldiers {

class Soldier {
    public:
        Soldier() noexcept:
            name_(), rang_(undefined), uniform_(), skills_(), spec_(std::make_unique<Fighter>()) {}

        Soldier(std::string&& name, Rang rang, unsigned int uniform,
        std::vector<Skill>&& skills, std::unique_ptr<SpecType>&& spec) noexcept:
            name_(std::move(name)), rang_(rang), uniform_(uniform), skills_(std::move(skills)), spec_(std::move(spec)) {}

//        Soldier(const Soldier& other):
//            name_(other.name_), rang_(other.rang_), uniform_(other.uniform_), 
//            skills_(other.skills_), spec_(other.spec_) {}

        Soldier(Soldier&& other) noexcept:
            name_(std::move(other.name_)), rang_(other.rang_), uniform_(other.uniform_), 
            skills_(std::move(other.skills_)), spec_(std::move(other.spec_)) {}

        void changeSpec(std::unique_ptr<SpecType>&& spec) noexcept;

        void setName(std::string&& other) noexcept;
        void setRang(Rang other) noexcept;
        void setUniform(unsigned int other) noexcept;
        void setSkills(std::vector<Skill>&& other) noexcept;
        void setSpec(std::unique_ptr<SpecType>&& other) noexcept;

        const std::string& getName() noexcept;
        const Rang& getRang() noexcept;
        const unsigned int& getUniform() noexcept;
        const std::vector<Skill>& getSkills() noexcept;
        const SpecType& getSpec() noexcept;

        friend void swap(Soldier& first, Soldier& second) noexcept;

    private:
        std::string name_;
        Rang rang_;
        unsigned int uniform_;
        std::vector<Skill> skills_;
        std::unique_ptr<SpecType> spec_;
};

void swap(Soldier& first, Soldier& second) noexcept;

}

#endif