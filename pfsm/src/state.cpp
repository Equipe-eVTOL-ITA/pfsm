#include "pfsm/state.hpp"

using namespace pfsm;

void State::on_enter(Blackboard &blackboard) {
    (void)blackboard;
}

void State::on_exit(Blackboard &blackboard) {
    (void)blackboard;
}

std::string State::to_string() {
    return "";
}