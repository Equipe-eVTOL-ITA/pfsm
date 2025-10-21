#pragma once

#include <string>

namespace pfsm {

    class Event {
    public:
        Event(std::string name) : name_{name} {}
    
        std::string to_string();

    private:

        std::string name_;

    };

};