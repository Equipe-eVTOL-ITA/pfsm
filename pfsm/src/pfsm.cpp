#include "pfsm/pfsm.hpp"
#include <iostream>
#include <stdexcept>

using namespace pfsm;

PFSM::PFSM(std::unordered_set<std::string> outcomes) : outcomes_{outcomes} {
    if (outcomes.empty())
        throw std::runtime_error("PFSM should have at least one outcome.");

    for (const auto &ss : outcomes)
        if (ss.empty())
            throw std::runtime_error("PFSM should not have empty outcome.");

    this->add_state("INITIAL", std::make_unique<InitialState>());
    this->current_state_ = "INITIAL";
    this->set_initial_state(*outcomes_.begin());
}

void PFSM::add_state(std::string name, std::unique_ptr<State> state) {
    
    this->states_.emplace(name, std::move(state));

    if (this->initial_state_not_set())
        this->set_initial_state(name);
}

void PFSM::add_transition(const std::string &current_state,
                         const std::string &transition, 
                         const std::string &next_state) 
{
    if (this->states_.count(current_state) == 0)
        throw std::runtime_error("State does not exist.");

    this->states_.at(current_state)->add_transition(transition, next_state);
}

void PFSM::add_transitions(const std::string &current_state, 
                          const std::unordered_map<std::string,std::string> &transitions)
{
    if (this->states_.count(current_state) == 0)
        throw std::runtime_error("State does not exist.");

    this->states_.at(current_state)->add_transitions(transitions);

}

bool PFSM::initial_state_not_set() {
    return (this->states_.at("INITIAL")->get_transitions().at("UNIQUE_TRANSITION")== *outcomes_.begin());
}


void PFSM::set_initial_state(std::string state_name) {
    this->states_.at("INITIAL")->transitions_.at("UNIQUE_TRANSITION") = state_name;
}

void PFSM::execute() {
    this->last_outcome_ = this->states_.at(this->current_state_)->act(this->blackboard_);
    this->check_transition();
}

void PFSM::check_transition() {

    // Continue in the same state
    if (this->last_outcome_.empty())
        return;

    auto state_transitions = this->states_.at(this->current_state_)->get_transitions();
    auto it = state_transitions.find(this->last_outcome_);

    if (it == state_transitions.end())
        throw std::runtime_error("Outcome [" + this->last_outcome_ + "] doesn't belong to current state [" + this->current_state_ + "].");


    // Launch onExit 
    this->states_.at(this->current_state_)->on_exit(blackboard_);

    // Change current_state_
    this->current_state_ = it->second;

    // Launch onEnter 
    if (this->is_finished())
        return;
    this->states_.at(this->current_state_)->on_enter(blackboard_);
}

bool PFSM::is_finished() {
    return (this->outcomes_.count(this->current_state_));
}

std::string PFSM::get_current_state() {
    return this->states_.at(this->current_state_)->to_string();
}

std::string PFSM::get_pfsm_outcome() {
    return this->current_state_;
}

/*
    InitialState implementation
*/

PFSM::InitialState::InitialState() : State() {
    this->add_transition("UNIQUE_TRANSITION", "");
}

std::string PFSM::InitialState::act(Blackboard &blackboard) {
    (void)blackboard;
    return "UNIQUE_TRANSITION";
}