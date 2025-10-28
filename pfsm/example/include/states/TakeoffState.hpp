using namespace pfsm;

class TakeoffState : public pfsm::State {
public:
    TakeoffState() = default;

    void on_enter(Blackboard& bb) override {
        (void)bb;
        printf("Entering Takeoff State\n");
    }

    std::string act(Blackboard& bb) override {
        (void)bb;
        printf("Taking off...\n");
        return "Flying";
    }

    void on_exit(Blackboard& bb) override {
        (void)bb;
        printf("Exiting Takeoff State\n");
    }
};