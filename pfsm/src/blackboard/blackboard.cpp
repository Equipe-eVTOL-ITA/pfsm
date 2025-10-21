#include "pfsm/blackboard/blackboard.hpp"

using namespace pfsm;

Blackboard::~Blackboard() {
    for (auto& [key, value] : values_)
        delete value;
}

bool Blackboard::contains(const std::string& key) {
    return (this->values_.find(key) != this->values_.end());
}