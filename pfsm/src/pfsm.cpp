#include "pfsm/pfsm.hpp"
#include <random>
#include <stdexcept>

using namespace pfsm;

void PFSM::set_initial_state(std::string state_name){
    this->current_state_ = state_name;
    this->initial_state_set_ = true;
}

void PFSM::add_state(std::string state_name, std::shared_ptr<State> state){
    this->states_.emplace(state_name, std::move(state));

    if(this->initial_state_set_ == false)
        this->set_initial_state(state_name);
}

void PFSM::add_transitions_by_event(std::string origin_state, EventTransitions event_transition){
    // checking if the probabilities sum up to 1.0
    for(const auto& event_pair : event_transition){
        float total_probability = 0.0f;
        for(const auto& state_prob_pair : event_pair.second){
            total_probability += state_prob_pair.second;
        }
        if(std::abs(total_probability - 1.0f) > 1e-5){
            throw std::runtime_error("Total probability for event [" + event_pair.first + "] in state [" + origin_state + "] does not sum up to 1.0.");
        }
    }

    this->tm_.emplace(origin_state, event_transition);
}

std::string PFSM::select_next_state(const std::string& trigger_event){
    const auto& map_state_prob = this->tm_.at(this->current_state_).at(trigger_event);
    if(map_state_prob.empty())
        throw std::runtime_error("No transitions defined for event [" + trigger_event + "] in state [" + this->current_state_ + "].");

    static thread_local std::mt19937 gen{std::random_device{}()};

    std::vector<float> probabilities;
    probabilities.reserve(map_state_prob.size());
    for (const auto& pair : map_state_prob) {
        probabilities.push_back(pair.second);
    }

    std::discrete_distribution<int> prob_distribution(probabilities.begin(), probabilities.end());
    int random_index = prob_distribution(gen);
    return map_state_prob[random_index].first;
}

std::string PFSM::get_current_state(){
    return this->current_state_;
}

void PFSM::execute(){
    if(this->entering){
        this->states_.at(this->current_state_)->on_enter(this->bb_);
        this->entering = false;
    }

    std::string trigger_event = this->states_.at(this->current_state_)->act(this->bb_);

    // continue running the same state
    if(trigger_event.empty()) return;

    auto it = this->tm_.at(this->current_state_).find(trigger_event);

    if(it == this->tm_.at(this->current_state_).end())
        throw std::runtime_error("Event [" + trigger_event + "] doesn't belong to current state [" + this->current_state_ + "].");

    // choosing (randomly) the next state according to probabilities
    std::string next_state = this->select_next_state(trigger_event);
    this->states_.at(this->current_state_)->on_exit(this->bb_);

    // transition to next state
    this->current_state_ = next_state;
    this->entering = true;
}