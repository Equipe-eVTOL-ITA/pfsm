The core difference between FSM and PFSM lies in the transition function ( $\delta$ ).

In a standard FSM, the $\delta$ is deterministic: $\delta(S_{current}, E) \rightarrow S_{next}$

In a PFSM, the transition is probabilistic. Given a current state and an input event, the function returns a probability distribution over the set of all possible next states.

$\delta(S_{current}, E) \rightarrow \{ (p_1, S_1), (p_2, S_2), ..., (p_n, S_n) \}$

(The sum of the probabilities for all possible outcomes must equal 1)

A structure is needed to store those probabilities. This structure is called Probabilistic Transition Matrix. But for a system where transitions are triggered by events, a simple matrix is not enough.

The structure for such a PFSM to work needs to answer the question: “From this state, given this event, what are the chances of choosing each possible next state?”. That could be a Map or a dictionary.

```cpp
Map<CurrentState, Map<Event, Map<NextState, Probability>>>
```

For example:

- Current state: TAKEOFF
- Event: altitude_reached
- Possible next states & Probabilities:
    - HOVER: 0.98
    - EMERGENCY_FAIL: 0.02

```cpp
transition_matrix = {
  "TAKEOFF": {
    "altitude_reached": {
      "HOVER": 0.98,
      "EMERGENCY_FAIL": 0.02
    },
    "battery_low": {
      "EMERGENCY_LANDING": 1.00
    }
  },
  "HOVER": {
    "waypoint_received": {
      "NAVIGATING": 0.99,
      "HOVER": 0.01  // Self-loop: e.g., command failed to process
    }
  }
  // ... and so on for all states and events
}
```

But how does the machine actually decide where to go?

It uses the probabilities to make a weighted random choice (sampling from a probability distribution).

---

### States

Distinct, high-level modes of operation. They should be human-readable and describe what the drone is doing

pfsm::State

### Events

Meaningful occurrences that trigger a change in behavior.

pfsm::Event

### Probabilities

Represent the reliability and uncertainty of the drone’s actions in the real world.

### PFSM Class

- Members:
    - State current_state
    - TransitionMatrix transition_matrix;
- Methods:
    - on_enter
    - act
    - on_exit
    - load_probabilities
        
        A method to load the transition matrix from a file (YAML), which was generated from the Monte Carlo simulations.[
        

### Blackboard

Same ideia as the blackboard from the fsm.