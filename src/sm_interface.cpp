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

void smInterface::complete_cb(const behaviors::completed::ConstPtr& activate) {
    std::cout<<"in complete_cb"<<std::endl;

    std::string act_beh = activate->behavior;
    std::string sub_beh = stateMachine_.get_state().first;
    bool isState = (act_beh.compare(sub_beh) != 0);

    if (!activate->yes && isState) {
        std::cout<<"data is false"<<std::endl;
        return;
    } else {
        std::cout<<"in else"<<std::endl;
        /************************in previous state******************************/
        pair<string, char> current_state = stateMachine_.get_state();
        std::cout<<"got current state as "<< current_state.first << std::endl;
        // char* msg[] = 'STATEMACHINE: ' + char(current_state.first) + ' completed successfully';
        // ROS_INFO(msg);

        //probably not going to use since transition is no longer void
        //stateMachine_.transition();  // transition to the next state

        /*************************in new state**********************************/
        //pair<string, char> target_state = stateMachine_.get_state();
        pair<string, char> target_state = stateMachine_.transition();
        std::cout<<"transitioned"<<std::endl;
        std::cout<<"Current State after transitioned "<<target_state.first<<std::endl;

        // NODELET_INFO("STATEMACHINE: transitioning to state " + target_state.first);

        /* stop current state */
        behaviors::target target_msg;
        target_msg.activate.data = false;
        pub_list_.at(curr_state_).publish(target_msg);

        std::cout<<"deactivated old state"<<std::endl;

        /* start next state */
        string state_string = "/" + target_state.first +"/activate";
        target_msg.activate.data = true;
        target_msg.goal.data = target_state.second;

        curr_state_ = 0;
        for(auto it : pub_list_) {
            std::string topic = it.getTopic();
            if(state_string.compare(topic) == 0) {
                std::cout<<"State_string = " << state_string << " topic " << topic <<std::endl;
                it.publish(target_msg);
                break;
            }
            curr_state_++;
            std::cout<<"we beepin"<<std::endl;
            //std::cout<<"NOT IN IF State_string = " << state_string << " topic " << topic <<std::endl;
            //std::cout<<"curr_state_ in interface.cpp in loop is " << curr_state_<< std::endl;
        }
        
        std::cout<<"sent activate message"<<std::endl;
    }
}


void smInterface::targetChange_cb(const std_msgs::Char::ConstPtr& target){
    //hard coding this BITCH IN
    if(stateMachine_.get_all_states().size() >= 8){
        char new_target = target->data;
        std::cout<<"changing that TARGET to " << new_target << std::endl;
        stateMachine_.change_target(8, new_target);
    }
    else{
        std::cout <<"Not changing target because no OR because the size is not >=8 because trevor said" << std::endl;
    }

}

void smInterface::check_time() {
    std::chrono::time_point<std::chrono::system_clock> nowish = std::chrono::system_clock::now();

    float diff = 
        chrono::duration_cast<chrono::milliseconds>(nowish - start_time_).count() / 1000;

    // print_msg(to_string(nowish.time_since_epoch().count()) + " and " +to_string(start_time_.time_since_epoch().count()) );
    // print_msg(to_string(diff));
    // send status
    int secs = int(floor(diff));
    if (secs >= 4.6 * 60) {
        /************************in previous state******************************/
        pair<string, char> current_state = stateMachine_.get_state();
        std::cout<<"got current state as "<< current_state.first << std::endl;
        // char* msg[] = 'STATEMACHINE: ' + char(current_state.first) + ' completed successfully';
        // ROS_INFO(msg);

        //probably not going to use since transition is no longer void
        //stateMachine_.transition();  // transition to the next state

        /*************************in new state**********************************/
        //pair<string, char> target_state = stateMachine_.get_state();

        // hard coding to return to start
        pair<string, char> target_state = stateMachine_.get_state_at(9);//stateMachine_.transition();
        std::cout<<"transitioned"<<std::endl;
        std::cout<<"Current State after transitioned "<<target_state.first<<std::endl;

        // NODELET_INFO("STATEMACHINE: transitioning to state " + target_state.first);

        /* stop current state */
        behaviors::target target_msg;
        target_msg.activate.data = false;
        pub_list_.at(curr_state_).publish(target_msg);

        std::cout<<"deactivated old state"<<std::endl;

        /* start next state */
        string state_string = "/" + target_state.first +"/activate";
        target_msg.activate.data = true;
        target_msg.goal.data = target_state.second;

        curr_state_ = 0;
        for(auto it : pub_list_) {
            std::string topic = it.getTopic();
            if(state_string.compare(topic) == 0) {
                std::cout<<"State_string = " << state_string << " topic " << topic <<std::endl;
                it.publish(target_msg);
                break;
            }
            curr_state_++;
            std::cout<<"we beepin"<<std::endl;
            //std::cout<<"NOT IN IF State_string = " << state_string << " topic " << topic <<std::endl;
            //std::cout<<"curr_state_ in interface.cpp in loop is " << curr_state_<< std::endl;
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



    target_change = nh_.subscribe<std_msgs::Char>("PPLocation", 1000, &smInterface::targetChange_cb, this);
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
            ros::Subscriber temp_sub = nh_.subscribe(it + "/completed", 1000, &smInterface::complete_cb, this);
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

    // wait 5 seconds to publish
    start_time_ = std::chrono::system_clock::now();
    std::chrono::duration<float> diff = 
        std::chrono::system_clock::now() - start_time_;
    while(diff.count() < 5)
        diff = std::chrono::system_clock::now() - start_time_;

    pub_list_.at(0).publish(begin);   

    std::cout<<"waiting"<<std::endl; 

    // give a thread to check the timer
    deviceThread_ = boost::shared_ptr< boost::thread > 
            (new boost::thread(boost::bind(&smInterface::check_time, this)));
    start_time_ = std::chrono::system_clock::now();
    ///init/activate
    ///init/activate

}


} // namespace statemachine