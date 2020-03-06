#include <openclx/bits/macros>
#include <openclx/downcast>
#include <openclx/intel/device>

#if CL_TARGET_VERSION >= 120 && \
    defined(cl_intel_device_partition_by_names) && \
    defined(CLX_HAVE_clCreateSubDevices)
std::vector<clx::intel::device>
clx::intel::device::partition_by_names(const std::vector<unsigned int>& names) const {
    std::vector<partition_property_type> prop;
    prop.reserve(3 + names.size());
    prop.emplace_back(CL_DEVICE_PARTITION_BY_NAMES_INTEL);
    for (auto n : names) {
        prop.emplace_back(partition_property_type(n));
    }
    prop.emplace_back(CL_PARTITION_BY_NAMES_LIST_END_INTEL);
    prop.emplace_back(CL_PROPERTIES_LIST_END_EXT);
    std::vector<device> result(names.size());
    unsigned_int_type actual_size = 0;
    CLX_CHECK(::clCreateSubDevices(
        this->_ptr, prop.data(),
        result.size(), downcast(result.data()),
        &actual_size
    ));
    result.resize(actual_size);
    return result;
}
#endif

#if CL_TARGET_VERSION >= 120 && defined(cl_intel_device_side_avc_motion_estimation)
CLX_METHOD_SCALAR(
    clx::intel::device::video_motion_estimation_version,
    ::clGetDeviceInfo,
    unsigned_int_type,
    CL_DEVICE_AVC_ME_VERSION_INTEL
);
CLX_METHOD_BOOL(
    clx::intel::device::supports_texture_sampler,
    ::clGetDeviceInfo,
    CL_DEVICE_AVC_ME_SUPPORTS_TEXTURE_SAMPLER_USE_INTEL
);
CLX_METHOD_BOOL(
    clx::intel::device::supports_preemption,
    ::clGetDeviceInfo,
    CL_DEVICE_AVC_ME_SUPPORTS_PREEMPTION_INTEL
);
#endif

#if CL_TARGET_VERSION >= 120 && defined(cl_intel_planar_yuv)
CLX_METHOD_SCALAR(
    clx::intel::device::planar_yuv_image_max_width,
    ::clGetDeviceInfo,
    size_t,
    CL_DEVICE_PLANAR_YUV_MAX_WIDTH_INTEL
);
CLX_METHOD_SCALAR(
    clx::intel::device::planar_yuv_image_max_height,
    ::clGetDeviceInfo,
    size_t,
    CL_DEVICE_PLANAR_YUV_MAX_HEIGHT_INTEL
);
#endif

#if CL_TARGET_VERSION >= 120 && defined(cl_intel_required_subgroup_size)
CLX_METHOD_ARRAY(
    clx::intel::device::supported_sub_group_sizes,
    ::clGetDeviceInfo,
    CL_DEVICE_SUB_GROUP_SIZES_INTEL,
    size_t
)
#endif

#if CL_TARGET_VERSION >= 120 && defined(cl_intel_simultaneous_sharing)
CLX_METHOD_SCALAR(
    clx::intel::device::num_simultaneous_graphics,
    ::clGetDeviceInfo,
    unsigned_int_type,
    CL_DEVICE_NUM_SIMULTANEOUS_INTEROPS_INTEL
);
CLX_METHOD_ARRAY(
    clx::intel::device::simultaneous_graphics_combinations,
    ::clGetDeviceInfo,
    CL_DEVICE_SIMULTANEOUS_INTEROPS_INTEL,
    unsigned_int_type
)
#endif

