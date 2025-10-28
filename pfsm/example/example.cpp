#include "pfsm/pfsm.hpp"
#include "TakeoffState.hpp"

class Mission : public pfsm::PFSM {
public:
    Mission(){
        // add states
        this->add_state("Takeoff", std::make_shared<TakeoffState>());

        // set initial state
        this->set_initial_state("Takeoff");

        // add transitions
        this->add_transitions_by_event("Takeoff", {
            {"takenoff", { 
                {"land", 0.1f},
                {"hover", 0.6f},
                {"fly", 0.3f}
            }},
            {"hover", {
                {"land", 0.3f},
                {"fly", 0.7f}
            }},
            {"fly", {
                {"land", 0.2f},
                {"hover", 0.5f},
                {"fly", 0.3f}
            }},
            {"land", {
                {"landed", 1.0f}
            }},
            {"landed", {
                {"mission_complete", 1.0f}
            }},
            {"abort", {
                {"land", 1.0f}
            }}
        });

    }
};

int main(){
    Mission mission;

    // execute the PFSM for a number of iterations
    for(int i = 0; i < 10; ++i){
        printf("Current State: %s\n", mission.get_current_state().c_str());
        mission.execute();
    }

    return 0;
}