#include <interface/sm_interface.h>

// export
PLUGINLIB_EXPORT_CLASS(statemachine::smInterface, nodelet::Nodelet);

namespace statemachine {
smInterface::smInterface() {
    curr_state_ = -1;
    state_machine_model_ = -1;
}

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
    if (!activate->data) {
        return;
    } else {
        /************************in previous state******************************/
        pair<string, char> current_state = stateMachine_.get_state();
        // char* msg[] = 'STATEMACHINE: ' + char(current_state.first) + ' completed successfully';
        // ROS_INFO(msg);

        stateMachine_.transition();  // transition to the next state

        /*************************in new state**********************************/
        pair<string, char> target_state = stateMachine_.get_state();

        // NODELET_INFO("STATEMACHINE: transitioning to state " + target_state.first);

        /* stop current state */
        behaviors::target target_msg;
        target_msg.activate.data = false;
        pub_list_.at(curr_state_).publish(target_msg);

        /* start next state */
        string state_string = target_state.first;
        target_msg.activate.data = true;
        target_msg.goal.data = target_state.second;
        curr_state_ = 0;
        for(auto it : pub_list_) {
            if(it.getTopic() == state_string) {
                it.publish(target_msg);
                break;
            }
            curr_state_++;
        }
        
    }
}

void smInterface::onInit() {
    /* parse launch file for which state machine to use */
    nh_.getParam("/StateMachineModel", state_machine_model_);
    bool successful_creating = createSm(state_machine_model_);
    if (! successful_creating) {
        // NODELET_INFO("STATEMACHINE: Tried creating statemachine with model number %d. This is INVALID, statemachine nodelet is gonna die now.",
        //     state_machine_model_);
        return;
    }

    // NODELET_INFO("STATEMACHINE: Successfully statemachine with model: %d", state_machine_model_);

    /* loop through all of the states and set publishers and subscribers */
    vector<string> state_list = stateMachine_.get_all_states();
    for (auto it : state_list) {
        ros::Subscriber temp_sub = nh_.subscribe<std_msgs::Bool>
                            (it + "/complete", 1000, &smInterface::complete_cb, this);
        ros::Publisher temp_pub = nh_.advertise<behaviors::target>
                            (it + "/activate", 1000);

        sub_list_.push_back(temp_sub);
        pub_list_.push_back(temp_pub);
    }


}
} // namespace statemachine