#include <statemachine/base_state_machine.h>
#include <iostream>

namespace statemachine {


    //add states from interface
    void stateMachine::add_states(pair<string, char> new_state){
        states.push_back(new_state);
    }

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
        //cout <<"state counter before if " << curr_state <<endl;
        if(curr_state > states.size()){            //Guess who's dumbass forgot that this wasn't python and I needed { }
            curr_state = 0;
            cout <<"I broke? then state size is? " << states.size() << "and curr_state is " << curr_state <<endl;
            return states.at(curr_state);
        }

        //increment's the object's current index 
        curr_state++;
        std::cout <<"increment that bitch to " << curr_state <<endl;
        return states.at(curr_state);
    }

    pair<string, char> stateMachine::get_state(){
        return states.at(curr_state);
    }

    //changes the target state for any given state at given index
    //EX: If state A is suppose to transition to State B, we can change state A to transition to state C using State A's index
    void stateMachine::change_target(int index, char target){
        states.at(index).second = target;
    }

    pair<string, char>stateMachine::get_state_at(int index) {
        if (index <= states.size()) {
            return states.at(index);
        }
    }


} // state_machine namespace