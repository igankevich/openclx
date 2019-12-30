#include <openclx/bits/macros>
#include <openclx/nv/device>

CLX_METHOD_SCALAR(
    clx::nv::device::compute_capability_major,
    ::clGetDeviceInfo,
    unsigned_int_type,
    CL_DEVICE_COMPUTE_CAPABILITY_MAJOR_NV
);

CLX_METHOD_SCALAR(
    clx::nv::device::compute_capability_minor,
    ::clGetDeviceInfo,
    unsigned_int_type,
    CL_DEVICE_COMPUTE_CAPABILITY_MINOR_NV
);

CLX_METHOD_SCALAR(
    clx::nv::device::max_registers,
    ::clGetDeviceInfo,
    unsigned_int_type,
    CL_DEVICE_REGISTERS_PER_BLOCK_NV
);

CLX_METHOD_SCALAR(
    clx::nv::device::warp_size,
    ::clGetDeviceInfo,
    unsigned_int_type,
    CL_DEVICE_WARP_SIZE_NV
);

CLX_METHOD_BOOL(
    clx::nv::device::overlap,
    ::clGetDeviceInfo,
    CL_DEVICE_GPU_OVERLAP_NV
);

CLX_METHOD_BOOL(
    clx::nv::device::kernel_execution_timeout,
    ::clGetDeviceInfo,
    CL_DEVICE_KERNEL_EXEC_TIMEOUT_NV
);

CLX_METHOD_BOOL(
    clx::nv::device::integrated_memory,
    ::clGetDeviceInfo,
    CL_DEVICE_INTEGRATED_MEMORY_NV
);
