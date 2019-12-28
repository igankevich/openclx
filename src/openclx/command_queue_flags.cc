#include <openclx/command_queue_flags>
#include <openclx/extensions>

std::vector<clx::queue_properties_type>
clx::command_queue_properties::operator()(const extensions& extensions) const {
    std::vector<clx::queue_properties_type> prop;
    prop.emplace_back(CL_QUEUE_PROPERTIES);
    prop.emplace_back(queue_properties_type(flags()));
    #if defined(CL_QUEUE_SIZE)
    prop.emplace_back(CL_QUEUE_SIZE);
    prop.emplace_back(queue_properties_type(size()));
    #endif
    #if defined(CL_QUEUE_PRIORITY_KHR)
    if (extensions("cl_khr_priority_hints")) {
        prop.emplace_back(CL_QUEUE_PRIORITY_KHR);
        prop.emplace_back(queue_properties_type(priority()));
    }
    #endif
    #if defined(CL_QUEUE_THROTTLE_KHR)
    if (extensions("cl_khr_throttle_hints")) {
        prop.emplace_back(CL_QUEUE_THROTTLE_KHR);
        prop.emplace_back(queue_properties_type(throttle()));
    }
    #endif
    prop.emplace_back(0);
    return prop;
}

