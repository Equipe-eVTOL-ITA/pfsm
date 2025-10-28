#pragma once

#include "pfsm/state.hpp"
#include "pfsm/event.hpp"
#include "pfsm/blackboard/blackboard.hpp"

#include <unordered_map>
#include <vector>
#include <string>
#include <memory>

typedef std::vector<std::pair<std::string, float>>              MapStateProbability;
typedef std::unordered_map<std::string, MapStateProbability>    EventTransitions;
typedef std::unordered_map<std::string, EventTransitions>       TransitionMatrix;

namespace pfsm {

    class PFSM {
    
    private:

        Blackboard bb_;
        std::unordered_map<std::string, std::shared_ptr<State>> states_;
        std::string current_state_;
        TransitionMatrix tm_;

        bool initial_state_set_ = false;
        bool entering = true;

    std::string select_next_state(const std::string& trigger_event);

    public:
        PFSM() = default;
        ~PFSM() = default;

        void add_state(std::string state_name, std::shared_ptr<State> state);
        void set_initial_state(std::string state_name);
    void add_transitions_by_event(std::string origin_state, EventTransitions event_transition);

        void execute();

        std::string get_current_state();

        template <typename T>
        void blackboard_set(const std::string & key, T* value){
            this->bb_.set<T>(key, value);
        }

        template <typename T>
        T* blackboard_get(const std::string & key){
            return this->bb_.get<T>(key);
        } 
    };

};