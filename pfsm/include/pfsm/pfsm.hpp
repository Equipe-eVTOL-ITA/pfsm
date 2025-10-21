#pragma once

#include "pfsm/state.hpp"
#include "pfsm/blackboard/blackboard.hpp"

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <memory>

namespace pfsm {

    class PFSM {
    public:
        PFSM() = default;
        ~PFSM() = default;

        void add_state(std::string state_name, std::unique_ptr<State> state);

        void add_transition(const std::string & current_state,
                            const std::string & transition,
                            const std::string & next_state);

        void add_transitions(const std::string & current_state,
                             const std::unordered_map<std::string, std::string> & transitions);
        
        void set_initial_state(std::string state_name);

        void execute();

        bool is_finished();

        std::string get_current_state();

        std::string get_pfsm_outcome();

        template <typename T>
        void blackboard_set(const std::string & key, T* value){
            this->bb_.set<T>(key, value);
        }

        template <typename T>
        T* blackboard_get(const std::string & key){
            return this->bb_.get<T>(key);
        }
    
    private:
        
        void check_transition();
        bool initial_state_not_set();

        Blackboard bb_;
        
        std::unordered_map<std::string, std::unique_ptr<State>> states_;
        std::unordered_set<std::string> outcomes_;
        std::string current_state_;
        std::string last_outcome_;

        class InitialState : public State {
        public:
            InitialState();
            std::string act(Blackboard &blackboard) override;
        };
        
    };

};