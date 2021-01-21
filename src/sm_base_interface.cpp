#include <interface/sm_base_interface.h>

// export
PLUGINLIB_EXPORT_CLASS(statemachine::smBaseInterface, nodelet::Nodelet);

namespace statemachine {
void smBaseInterface::onInit() {
    // creat thread for the control loop
    running_ = true;
    deviceThread_ = boost::shared_ptr< boost::thread > 
        (new boost::thread(boost::bind(&smBaseInterface::control, this)));

    // subscribe topic: sm_switch type: std_msgs/String

    // publishers topic: <state_name>/activate type: std_msgs/Bool
}
void smBaseInterface::control() {
    while(running_) {
    std::cout<<"testing\n";
        // check if current state is different from wanted state
        // check if wanted transition is legal 
            // if not legal: switch wanted state -> current
                // return 
            // else: proceed
        // turn off old state by sending false
        // turn on new state by sending true 
    }
}
} // namespace statemachine