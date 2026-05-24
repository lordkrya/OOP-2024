#include "soldier.hpp"

namespace soldiers {

void Soldier::changeSpec(std::unique_ptr<SpecType>&& spec) noexcept {
    spec_ = std::move(spec);
}

void Soldier::setName(std::string&& other) noexcept { name_ = std::move(other); }
void Soldier::setRang(Rang other) noexcept { rang_ = other; }
void Soldier::setUniform(unsigned int other) noexcept { uniform_ = other; }
void Soldier::setSkills(std::vector<Skill>&& other) noexcept { skills_ = std::move(other); }
void Soldier::setSpec(std::unique_ptr<SpecType>&& other) noexcept { spec_ = std::move(other); }

const std::string& Soldier::getName() noexcept {return name_;}
const Rang& Soldier::getRang() noexcept {return rang_;}
const unsigned int& Soldier::getUniform() noexcept {return uniform_;}
const std::vector<Skill>& Soldier::getSkills() noexcept {return skills_;}
const SpecType& Soldier::getSpec() noexcept {return *spec_;}

void swap(Soldier& first, Soldier& second) noexcept {
    std::swap(first.name_, second.name_);
    std::swap(first.rang_, second.rang_);
    std::swap(first.uniform_, second.uniform_);
    std::swap(first.skills_, second.skills_);
    std::swap(first.spec_, second.spec_);
}

}
