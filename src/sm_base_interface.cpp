#include <interface/sm_base_interface.h>

// export
PLUGINLIB_EXPORT_CLASS(statemachine::smBaseInterface, nodelet::Nodelet);

namespace statemachine {
void smBaseInterface::onInit() {
    // creat thread for the control loop
    running_ = true;
    deviceThread_ = boost::shared_ptr< boost::thread > 
        (new boost::thread(boost::bind(&smBaseInterface::control, this)));
}
void smBaseInterface::control() {
    while(running_) {
    std::cout<<"testing\n";
    }
}
} // namespace statemachine