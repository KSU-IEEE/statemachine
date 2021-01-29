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

namespace statemachine {
class smBaseInterface : public nodelet::Nodelet {
public:
    void onInit() override;
    void control();

private:
    // from
    //https://github.com/ros-drivers/camera1394/blob/abc1950a6925628acc7581cabc312741706b6e4c/src/nodes/nodelet.cpp 
    volatile bool running_;               ///< device is running
    boost::shared_ptr<boost::thread> deviceThread_;
public:
};
} // namespace statemachine 
#endif // SM_INTERFACE_H