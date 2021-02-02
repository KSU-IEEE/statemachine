#ifndef BASE_STATE_MACHINE_H 
#define BASE_STATE_MACHINE_H

// includes 
#include <utility> // std::pair
#include <vector>
#include <string>

using namespace std;
namespace state_machine {

class stateMachine {
public:
    void transition();
    pair<string, char> get_state();
    vector<string> get_all_states();
    
private:
    vector<pair<string, char>> states;
    int curr_state;  // index in vector above
protected:
}; //class state
} // namespace statemachine
#endif // BASE_STATE_MACHINE_H