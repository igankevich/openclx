#include <openclx/arm/device>
#include <openclx/bits/macros>

#if CL_TARGET_VERSION >= 120 && defined(cl_arm_shared_virtual_memory)
CLX_METHOD_SCALAR(
    clx::arm::device::supported_svm_flags,
    ::clGetDeviceInfo,
    svm_flags,
    CL_DEVICE_SVM_CAPABILITIES_ARM
);
#endif

#if defined(cl_arm_core_id) && CL_TARGET_VERSION >= 120
CLX_METHOD_SCALAR(
    clx::arm::device::compute_units_mask,
    ::clGetDeviceInfo,
    unsigned_long_type,
    CL_DEVICE_COMPUTE_UNITS_BITFIELD_ARM
);
#endif

