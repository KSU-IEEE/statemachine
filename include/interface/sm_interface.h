#ifndef SM_BASE_INTERFACE_H
#define SM_BASE_INTERFACE_H 
/***********************************************************************
State Machine interface nodelet: 

This nodelet is meant to control the state machine for various different
projects. the main goal of this is to be able to have a project agnostic
statemachine, that can easily be inherited from to to run the 
statemachine, simply by telling it which statemachine file to use
************************************************************************/
// nodelet includes
#include <ros/ros.h>
#include <nodelet/nodelet.h>
#include <pluginlib/class_list_macros.h>

// random libraries
#include <boost/thread.hpp>

// put messages here
#include <behaviors/target.h>
#include <behaviors/completed.h>
#include <std_msgs/Bool.h>

//statemachines
#include <statemachine/base_state_machine.h>

namespace statemachine {
class smInterface : public nodelet::Nodelet {
public:
    // initializers
    smInterface();
    ~smInterface();

    void onInit() override;

    bool createSm(int target);  // get which state machine to to use (see list above)
    void complete_cb(const behaviors::completed::ConstPtr& activate);
    void transition_cb(const std_msgs::Bool::ConstPtr& yes) {if(yes->data)stateMachine_.transition();}

private:
    // list of publishers
    vector<ros::Publisher> pub_list_;
    vector<ros::Subscriber> sub_list_;

    ros::Subscriber sub_transition;

    ros::NodeHandle nh_;

    int curr_state_;
    int state_machine_model_;

    // statemachines
    stateMachine stateMachine_;
};
} // namespace statemachine 
#endif // SM_INTERFACE_H