#pragma once

#include "pfsm/blackboard/blackboard.hpp"

#include <string>

namespace pfsm {

    class State {
    public:
        State() = default;
    
    private:

        virtual std::string act(Blackboard &blackboard) = 0;
        virtual void on_enter(Blackboard &blackboard);
        virtual void on_exit(Blackboard &blackboard);
        virtual std::string to_string();
    
    friend class PFSM;

    };

};