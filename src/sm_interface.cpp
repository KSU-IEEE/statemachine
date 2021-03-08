#include <interface/sm_interface.h>

// export
PLUGINLIB_EXPORT_CLASS(statemachine::smInterface, nodelet::Nodelet);

namespace statemachine {
smInterface::smInterface() {
    curr_state_ = 0;
    state_machine_model_ = -1;
}

smInterface::~smInterface(){}

bool smInterface::createSm(int target){
    // enter new state machines here, just increment the int
    switch(target){
        case 0: 
            stateMachine_ = stateMachine();
            return true;
        case 1: 
            // pac-man-bot sm set here
            return true;
        default:
            return false;
    }
}

void smInterface::complete_cb(const std_msgs::Bool::ConstPtr& activate) {
    std::cout<<"in complete_cb"<<std::endl;

    std::string act_beh = activate.behavior;
    std::string sub_beh = stateMachine_.get_state().first;
    bool isState = (act_topic.compare(sub_beh) != 0);

    if (!activate->yes && isState) {
        std::cout<<"data is false"<<std::endl;
        return;
    } else {
        std::cout<<"in else"<<std::endl;
        /************************in previous state******************************/
        pair<string, char> current_state = stateMachine_.get_state();
        std::cout<<"got current state"<<std::endl;
        // char* msg[] = 'STATEMACHINE: ' + char(current_state.first) + ' completed successfully';
        // ROS_INFO(msg);

        //probably not going to use since transition is no longer void
        //stateMachine_.transition();  // transition to the next state

        /*************************in new state**********************************/
        //pair<string, char> target_state = stateMachine_.get_state();
        pair<string, char> target_state = stateMachine_.transition();
        std::cout<<"transitioned"<<std::endl;

        // NODELET_INFO("STATEMACHINE: transitioning to state " + target_state.first);

        /* stop current state */
        behaviors::target target_msg;
        target_msg.activate.data = false;
        pub_list_.at(curr_state_).publish(target_msg);

        std::cout<<"deactivated old state"<<std::endl;

        /* start next state */
        string state_string = target_state.first;
        target_msg.activate.data = true;
        target_msg.goal.data = target_state.second;

        curr_state_ = 0;
        for(auto it : pub_list_) {
            std::string topic = it.getTopic();
            if(state_string.compare(topic) != 0) {
                it.publish(target_msg);
                break;
            }
            curr_state_++;
        }
        
        std::cout<<"sent activate message"<<std::endl;
    }
}

void smInterface::onInit() {
    /* parse launch file for which state machine to use */
    nh_.getParam("/StateMachineModel", state_machine_model_);
    // bool successful_creating = createSm(state_machine_model_);

    // grab state definitions
    // std::vector<std::string> states;
    // std::vector<std::string> targets;
    std::string states;
    std::string targets;

    nh_.param("/statemachine", states, std::string());
    nh_.param("/targets", targets, std::string());

    std::cout<<"Size of states: "<<states.size()<<std::endl;
    std::cout<<"Targets size: "<< targets.size()<<std::endl;

    std::string built_state = "";
    int currTarget = 0;

    for (int i = 0; i < states.size(); i++) {
        if (states[i] == ' ') {
            std::pair<std::string, char> state;
            state.first = built_state;
            char target = targets[currTarget++];
            state.second = target; //char(target[0]);

            stateMachine_.add_states(state);

            built_state = "";
        } else {
            built_state.push_back(states[i]);
        }
    }

    // if (! successful_creating) {
        // NODELET_INFO("STATEMACHINE: Tried creating statemachine with model number %d. This is INVALID, statemachine nodelet is gonna die now.",
        //     state_machine_model_);
    //     return;
    // }

    sub_transition = nh_.subscribe<std_msgs::Bool>("sm/transition", 1000, &smInterface::transition_cb, this);
    // NODELET_INFO("STATEMACHINE: Successfully statemachine with model: %d", state_machine_model_);

    /* loop through all of the states and set publishers and subscribers */
    vector<string> state_list = stateMachine_.get_all_states();
    for (auto it : state_list) {
        // make sure I didn't make it already
        bool make = true;
        std::string tester = "/" + it + "/completed";
        for (auto sub_it : sub_list_) {
            std::string topic = sub_it.getTopic();
            if(tester.compare(topic) == 0) {
                make = false;
                break;
            } 

        }
        if(make) {
            std::cout<<"Initializing for state: "<<it<<std::endl;
            ros::Subscriber temp_sub = nh_.subscribe<std_msgs::Bool>
                                (it + "/completed", 1000, &smInterface::complete_cb, this);
            ros::Publisher temp_pub = nh_.advertise<behaviors::target>
                                (it + "/activate", 1000);

            sub_list_.push_back(temp_sub);
            pub_list_.push_back(temp_pub);
        }
    }

    std::cout<<"publishing to topic: "<<pub_list_.at(0).getTopic()<<std::endl;
    behaviors::target begin;
    begin.activate.data = true;
    // begin.goal.data= stateMachine_.get_state().second;
    pub_list_.at(0).publish(begin);   

    std::cout<<"waiting"<<std::endl; 

    ///init/activate
    ///init/activate

}
} // namespace statemachine