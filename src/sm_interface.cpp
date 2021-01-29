#include <interface/sm_interface.h>

// export
// PLUGINLIB_EXPORT_CLASS(statemachine::smBaseInterface, nodelet::Nodelet);

// namespace statemachine {
// void smBaseInterface::onInit() {
//     // creat thread for the control loop
//     running_ = true;
//     deviceThread_ = boost::shared_ptr< boost::thread > 
//         (new boost::thread(boost::bind(&smBaseInterface::control, this)));

//     // subscribe topic: sm_switch type: std_msgs/String

//     // publishers topic: <state_name>/activate type: std_msgs/Bool
// }

// }
// } // namespace statemachine