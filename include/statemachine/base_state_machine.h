#ifndef STATEMACHINE_H 
#define STATEMACHINE_H

// includes 
#include <utility> // std::pair
#include <vector>
#include <string>

using namespace std;
namespace statemachine {

class stateMachine {
public:
    stateMachine(){}
    ~stateMachine(){}

    pair<string, char> transition(); //returns the next state
    pair<string, char> get_state();  //get's current state
    vector<string> get_all_states(); //returns vector of all state's names

    void add_states(pair<string, char> state_name);

    //change target at given index
    //this is so we can change our target to 'A' or 'C' based on location of pellet
    void change_target(int index, char target);

private:
    vector<pair<string, char>> states;  //vector of every state
    int curr_state = 0;  // index in vector above
protected:
}; //class state
} // namespace statemachine
#endif // BASE_STATE_MACHINE_H