#include <statemachine/base_state_machine.h>


namespace statemachine {

    //returns a vector of all the states
    vector<string> stateMachine::get_all_states(){
        //new vector to hold just the name of the states
        vector<string> stateNames;

        //itereates through a vector of pairs and adds the first element of the pair to new vector
        for(vector<pair<string, char>>::iterator it = states.begin(); it != states.end(); it++){
            stateNames.push_back(it -> first);
        }
        return stateNames;
    }

    //returns the next state
    pair<string, char> stateMachine::transition(){
        //if, by chance, the bot tries to transition out of bounds, loop to beginning
        if(curr_state > states.size())
            curr_state = 0;
            return states.at(curr_state);

        //increment's the object's current index 
        curr_state++;
        return states.at(curr_state);
    }

    pair<string, char> stateMachine::get_state(){
        return states.at(curr_state);
    }

    //changes the target state for any given state at given index
    //EX: If state A is suppose to transition to State B, we can change state A to transition to state C using State A's index
    void stateMachine::change_index(int index, char target){
        states.at(index).second = target;
    }

} // state_machine namespace