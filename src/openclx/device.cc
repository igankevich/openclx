#include <openclx/bits/macros>
#include <openclx/command_queue>
#include <openclx/context>
#include <openclx/device>
#include <openclx/downcast>
#include <openclx/extensions>
#include <openclx/platform>

#if defined(cl_ext_device_fission)
#include <openclx/ext/device>
#include <openclx/ext/partition_properties>
#endif

#define CLX_DEVICE_PREFERRED_VECTOR_WIDTH(type, type2) \
    template <> \
    clx::unsigned_int_type \
    clx::device::preferred_vector_width<clx::type>() const { \
        CLX_BODY_SCALAR(::clGetDeviceInfo, unsigned_int_type, \
            CL_DEVICE_PREFERRED_VECTOR_WIDTH_##type2) \
    }

#define CLX_DEVICE_NATIVE_VECTOR_WIDTH(type, type2) \
    template <> \
    clx::unsigned_int_type \
    clx::device::native_vector_width<clx::type>() const { \
        CLX_BODY_SCALAR(::clGetDeviceInfo, unsigned_int_type, \
            CL_DEVICE_NATIVE_VECTOR_WIDTH_##type2) \
    }

CLX_METHOD_STRING(clx::device::name, ::clGetDeviceInfo, CL_DEVICE_NAME)
CLX_METHOD_STRING(clx::device::profile, ::clGetDeviceInfo, CL_DEVICE_PROFILE)
CLX_METHOD_STRING(clx::device::vendor, ::clGetDeviceInfo, CL_DEVICE_VENDOR)
CLX_METHOD_STRING(clx::device::version, ::clGetDeviceInfo, CL_DEVICE_VERSION)
CLX_METHOD_STRING(clx::device::driver_version, ::clGetDeviceInfo, CL_DRIVER_VERSION)
CLX_METHOD_STRING2(
    clx::device::extensions,
    ::clGetDeviceInfo,
    ::clx::extensions,
    CL_DEVICE_EXTENSIONS
)

#if defined(CL_DEVICE_SPIR_VERSIONS)
CLX_METHOD_STRING(
    clx::device::spir_versions,
    ::clGetDeviceInfo,
    CL_DEVICE_SPIR_VERSIONS
);
#endif

#if CL_TARGET_VERSION >= 110
CLX_METHOD_STRING(
    clx::device::c_version,
    ::clGetDeviceInfo,
    CL_DEVICE_OPENCL_C_VERSION
);
#endif

#if CL_TARGET_VERSION >= 120
CLX_METHOD_STRING(
    clx::device::builtin_kernels,
    ::clGetDeviceInfo,
    CL_DEVICE_BUILT_IN_KERNELS
);
#endif

#if CL_TARGET_VERSION >= 110 && defined(cl_qcom_ext_host_ptr)
CLX_METHOD_SCALAR(
    clx::device::memory_padding,
    ::clGetDeviceInfo,
    size_t,
    CL_DEVICE_EXT_MEM_PADDING_IN_BYTES_QCOM
);
CLX_METHOD_SCALAR(
    clx::device::page_size,
    ::clGetDeviceInfo,
    size_t,
    CL_DEVICE_PAGE_SIZE_QCOM
);
::clx::qcom::image_pitch
clx::device::image_pitch(size_t width, size_t height, const image_format& format) const {
    auto func = CLX_EXTENSION(clGetDeviceImageInfoQCOM, platform());
    ::clx::qcom::image_pitch pitch{};
    CLX_CHECK(func(
        this->_ptr, width, height, format,
        CL_IMAGE_ROW_PITCH, sizeof(unsigned_int_type), &pitch.row_pitch, nullptr
    ));
    CLX_CHECK(func(
        this->_ptr, width, height, format,
        CL_IMAGE_ROW_ALIGNMENT_QCOM, sizeof(unsigned_int_type), &pitch.row_alignment, nullptr
    ));
    CLX_CHECK(func(
        this->_ptr, width, height, format,
        CL_IMAGE_SLICE_PITCH, sizeof(unsigned_int_type), &pitch.slice_pitch, nullptr
    ));
    CLX_CHECK(func(
        this->_ptr, width, height, format,
        CL_IMAGE_SLICE_ALIGNMENT_QCOM, sizeof(unsigned_int_type), &pitch.slice_alignment,
        nullptr
    ));
    return pitch;
}
#endif

#if CL_TARGET_VERSION >= 210
auto
clx::device::host_time() const -> nanoseconds {
    unsigned_long_type value = 0;
    CLX_CHECK(::clGetHostTimer(this->_ptr, &value));
    return static_cast<nanoseconds>(value);
}
#endif

#if CL_TARGET_VERSION >= 210
auto
clx::device::device_and_host_time() const -> std::pair<nanoseconds,nanoseconds> {
    unsigned_long_type value1 = 0, value2 = 0;
    CLX_CHECK(::clGetDeviceAndHostTimer(this->_ptr, &value1, &value2));
    return std::make_pair(
        static_cast<nanoseconds>(value1),
        static_cast<nanoseconds>(value2)
    );
}
#endif

#if CL_TARGET_VERSION >= 210
CLX_METHOD_STRING(
    clx::device::intermediate_languages,
    ::clGetDeviceInfo,
    CL_DEVICE_IL_VERSION
)
#endif

CLX_METHOD_SCALAR(
    clx::device::timer_resolution,
    ::clGetDeviceInfo,
    nanoseconds,
    CL_DEVICE_PROFILING_TIMER_RESOLUTION
)

CLX_METHOD_SCALAR(
    clx::device::timer_offset,
    ::clGetDeviceInfo,
    nanoseconds,
    CL_DEVICE_PROFILING_TIMER_OFFSET_AMD
)

CLX_METHOD_SCALAR(
    clx::device::vendor_id,
    ::clGetDeviceInfo,
    unsigned_int_type,
    CL_DEVICE_VENDOR_ID
)

CLX_METHOD_SCALAR(
    clx::device::platform,
    ::clGetDeviceInfo,
    ::clx::platform,
    CL_DEVICE_PLATFORM
);

#if CL_TARGET_VERSION >= 120
CLX_METHOD_SCALAR(
    clx::device::num_references,
    ::clGetDeviceInfo,
    unsigned_int_type,
    CL_DEVICE_REFERENCE_COUNT
)
#endif

clx::context clx::device::context() const {
    std::vector<context_properties_type> prop{
        context_properties_type(CL_CONTEXT_PLATFORM),
        context_properties_type(this->platform().get()),
        context_properties_type(0)
    };
    int_type ret = 0;
    auto ctx = ::clCreateContext(prop.data(), 1, &this->_ptr, nullptr, nullptr, &ret);
    CLX_CHECK(ret);
    return static_cast<clx::context>(ctx);
}

#if CL_TARGET_VERSION >= 200
clx::command_queue
clx::device::queue_200(context_type ctx, const command_queue_properties& prop) const {
    const auto& props = prop(extensions());
    int_type ret = 0;
    auto result = ::clCreateCommandQueueWithProperties(ctx, this->_ptr, props.data(), &ret);
    CLX_CHECK(ret);
    return static_cast<::clx::command_queue>(result);
}
#endif

#if CL_TARGET_VERSION <= 120 || defined(CL_USE_DEPRECATED_OPENCL_1_2_APIS)
clx::command_queue
clx::device::queue_100(context_type ctx, const command_queue_properties& prop) const {
    int_type ret = 0;
    #if defined(cl_khr_create_command_queue)
    const auto& ext = extensions();
    command_queue_type result;
    if (ext("cl_khr_create_command_queue")) {
        auto func = CLX_EXTENSION(clCreateCommandQueueWithPropertiesKHR, platform());
        const auto& props = prop(extensions());
        result = func(ctx, this->_ptr, props.data(), &ret);
    } else
    #endif
    { result = ::clCreateCommandQueue(ctx, this->_ptr, downcast(prop.flags()), &ret); }
    CLX_CHECK(ret);
    return static_cast<::clx::command_queue>(result);
}
#endif

CLX_METHOD_BOOL(
    clx::device::available,
    ::clGetDeviceInfo,
    CL_DEVICE_AVAILABLE
);

CLX_METHOD_BOOL(
    clx::device::compiler_available,
    ::clGetDeviceInfo,
    CL_DEVICE_COMPILER_AVAILABLE
);

#if CL_TARGET_VERSION >= 120
CLX_METHOD_BOOL(
    clx::device::linker_available,
    ::clGetDeviceInfo,
    CL_DEVICE_LINKER_AVAILABLE
);
#endif

CLX_METHOD_BOOL(clx::device::little_endian, ::clGetDeviceInfo, CL_DEVICE_ENDIAN_LITTLE)
CLX_METHOD_BOOL(clx::device::supports_error_correction, ::clGetDeviceInfo, CL_DEVICE_ERROR_CORRECTION_SUPPORT)
CLX_METHOD_BOOL(clx::device::supports_images, ::clGetDeviceInfo, CL_DEVICE_IMAGE_SUPPORT);
#if CL_TARGET_VERSION >= 120
CLX_METHOD_BOOL(
    clx::device::prefers_user_sync,
    ::clGetDeviceInfo,
    CL_DEVICE_PREFERRED_INTEROP_USER_SYNC
);
#endif

#if CL_TARGET_VERSION >= 120
CLX_METHOD_SCALAR(
    clx::device::printf_buffer_size,
    ::clGetDeviceInfo,
    size_t,
    CL_DEVICE_PRINTF_BUFFER_SIZE
);
#endif

CLX_METHOD_SCALAR(
    clx::device::image2d_max_width,
    ::clGetDeviceInfo,
    size_t,
    CL_DEVICE_IMAGE2D_MAX_WIDTH
);

CLX_METHOD_SCALAR(
    clx::device::image2d_max_height,
    ::clGetDeviceInfo,
    size_t,
    CL_DEVICE_IMAGE2D_MAX_HEIGHT
);

#if CL_TARGET_VERSION >= 120
CLX_METHOD_SCALAR(
    clx::device::image2d_base_address_alignment,
    ::clGetDeviceInfo,
    unsigned_int_type,
    CL_DEVICE_IMAGE_BASE_ADDRESS_ALIGNMENT
);

CLX_METHOD_SCALAR(
    clx::device::image2d_pitch_alignment,
    ::clGetDeviceInfo,
    unsigned_int_type,
    CL_DEVICE_IMAGE_PITCH_ALIGNMENT
);

CLX_METHOD_SCALAR(
    clx::device::image_max_buffer_size,
    ::clGetDeviceInfo,
    size_t,
    CL_DEVICE_IMAGE_MAX_BUFFER_SIZE
);

CLX_METHOD_SCALAR(
    clx::device::image_max_array_size,
    ::clGetDeviceInfo,
    size_t,
    CL_DEVICE_IMAGE_MAX_ARRAY_SIZE
);

#endif

CLX_METHOD_SCALAR(clx::device::image3d_max_width, ::clGetDeviceInfo, size_t, CL_DEVICE_IMAGE3D_MAX_WIDTH)
CLX_METHOD_SCALAR(clx::device::image3d_max_height, ::clGetDeviceInfo, size_t, CL_DEVICE_IMAGE3D_MAX_HEIGHT)
CLX_METHOD_SCALAR(clx::device::image3d_max_depth, ::clGetDeviceInfo, size_t, CL_DEVICE_IMAGE3D_MAX_DEPTH)

CLX_METHOD_SCALAR(clx::device::global_memory_size, ::clGetDeviceInfo, unsigned_long_type, CL_DEVICE_GLOBAL_MEM_SIZE)
CLX_METHOD_SCALAR(clx::device::global_memory_cache_size, ::clGetDeviceInfo, unsigned_long_type, CL_DEVICE_GLOBAL_MEM_CACHE_SIZE)
CLX_METHOD_SCALAR(clx::device::global_memory_cacheline_size, ::clGetDeviceInfo, unsigned_int_type, CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE)
CLX_METHOD_SCALAR(clx::device::global_memory_cache_type, ::clGetDeviceInfo, device_memory_cache, CL_DEVICE_GLOBAL_MEM_CACHE_TYPE)
CLX_METHOD_SCALAR(clx::device::local_memory_size, ::clGetDeviceInfo, unsigned_long_type, CL_DEVICE_LOCAL_MEM_SIZE)
CLX_METHOD_SCALAR(clx::device::local_memory_type, ::clGetDeviceInfo, device_local_memory, CL_DEVICE_LOCAL_MEM_TYPE)

CLX_METHOD_SCALAR(clx::device::address_bits, ::clGetDeviceInfo, unsigned_int_type, CL_DEVICE_ADDRESS_BITS)
CLX_METHOD_SCALAR(clx::device::base_address_alignment, ::clGetDeviceInfo, unsigned_int_type, CL_DEVICE_MEM_BASE_ADDR_ALIGN)
CLX_METHOD_SCALAR(clx::device::min_data_alignment, ::clGetDeviceInfo, unsigned_int_type, CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE)

CLX_METHOD_SCALAR(clx::device::execution_capabilities, ::clGetDeviceInfo, execution_flags, CL_DEVICE_EXECUTION_CAPABILITIES)

template <>
clx::floating_point_flags
clx::device::floating_point_capabilities<clx::half_type>() const {
    CLX_BODY_SCALAR(::clGetDeviceInfo, floating_point_flags, CL_DEVICE_HALF_FP_CONFIG)
}

template <>
clx::floating_point_flags
clx::device::floating_point_capabilities<clx::float_type>() const {
    CLX_BODY_SCALAR(::clGetDeviceInfo, floating_point_flags, CL_DEVICE_SINGLE_FP_CONFIG)
}

template <>
clx::floating_point_flags
clx::device::floating_point_capabilities<clx::double_type>() const {
    CLX_BODY_SCALAR(::clGetDeviceInfo, floating_point_flags, CL_DEVICE_DOUBLE_FP_CONFIG)
}

CLX_DEVICE_PREFERRED_VECTOR_WIDTH(char_type, CHAR);
CLX_DEVICE_PREFERRED_VECTOR_WIDTH(short_type, SHORT);
CLX_DEVICE_PREFERRED_VECTOR_WIDTH(int_type, INT);
CLX_DEVICE_PREFERRED_VECTOR_WIDTH(long_type, LONG);
#if CL_TARGET_VERSION >= 110
CLX_DEVICE_PREFERRED_VECTOR_WIDTH(half_type, HALF);
#endif
CLX_DEVICE_PREFERRED_VECTOR_WIDTH(float_type, FLOAT);
CLX_DEVICE_PREFERRED_VECTOR_WIDTH(double_type, DOUBLE);

#if CL_TARGET_VERSION >= 110
CLX_DEVICE_NATIVE_VECTOR_WIDTH(char_type, CHAR);
CLX_DEVICE_NATIVE_VECTOR_WIDTH(short_type, SHORT);
CLX_DEVICE_NATIVE_VECTOR_WIDTH(int_type, INT);
CLX_DEVICE_NATIVE_VECTOR_WIDTH(long_type, LONG);
CLX_DEVICE_NATIVE_VECTOR_WIDTH(half_type, HALF);
CLX_DEVICE_NATIVE_VECTOR_WIDTH(float_type, FLOAT);
CLX_DEVICE_NATIVE_VECTOR_WIDTH(double_type, DOUBLE);
#endif

CLX_METHOD_SCALAR(clx::device::max_clock_frequency, ::clGetDeviceInfo, unsigned_int_type, CL_DEVICE_MAX_CLOCK_FREQUENCY)
CLX_METHOD_SCALAR(clx::device::max_compute_units, ::clGetDeviceInfo, unsigned_int_type, CL_DEVICE_MAX_COMPUTE_UNITS)

CLX_METHOD_SCALAR(clx::device::max_constant_arguments, ::clGetDeviceInfo, unsigned_int_type, CL_DEVICE_MAX_CONSTANT_ARGS)
CLX_METHOD_SCALAR(clx::device::max_read_image_arguments, ::clGetDeviceInfo, unsigned_int_type, CL_DEVICE_MAX_READ_IMAGE_ARGS)
CLX_METHOD_SCALAR(
    clx::device::max_write_image_arguments,
    ::clGetDeviceInfo,
    unsigned_int_type,
    CL_DEVICE_MAX_WRITE_IMAGE_ARGS
);

#if CL_TARGET_VERSION >= 200
CLX_METHOD_SCALAR(
    clx::device::max_read_write_image_arguments,
    ::clGetDeviceInfo,
    unsigned_int_type,
    CL_DEVICE_MAX_READ_WRITE_IMAGE_ARGS
);
CLX_METHOD_SCALAR(
    clx::device::max_pipe_arguments,
    ::clGetDeviceInfo,
    unsigned_int_type,
    CL_DEVICE_MAX_PIPE_ARGS
);
#endif

CLX_METHOD_SCALAR(clx::device::max_samplers, ::clGetDeviceInfo, unsigned_int_type, CL_DEVICE_MAX_SAMPLERS)
CLX_METHOD_SCALAR(clx::device::max_parameter_size, ::clGetDeviceInfo, size_t, CL_DEVICE_MAX_PARAMETER_SIZE)

CLX_METHOD_SCALAR(clx::device::max_constant_buffer_size, ::clGetDeviceInfo, unsigned_long_type, CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE)

CLX_METHOD_SCALAR(
    clx::device::max_memory_allocation_size,
    ::clGetDeviceInfo,
    unsigned_long_type,
    CL_DEVICE_MAX_MEM_ALLOC_SIZE
);

#if CL_TARGET_VERSION >= 200
CLX_METHOD_SCALAR(
    clx::device::max_global_variable_size,
    ::clGetDeviceInfo,
    unsigned_long_type,
    CL_DEVICE_MAX_GLOBAL_VARIABLE_SIZE
);
CLX_METHOD_SCALAR(
    clx::device::preferred_global_variable_size,
    ::clGetDeviceInfo,
    unsigned_long_type,
    CL_DEVICE_GLOBAL_VARIABLE_PREFERRED_TOTAL_SIZE
);
#endif

CLX_METHOD_SCALAR(clx::device::max_work_group_size, ::clGetDeviceInfo, size_t, CL_DEVICE_MAX_WORK_GROUP_SIZE)
CLX_METHOD_SCALAR(clx::device::max_work_item_dimensions, ::clGetDeviceInfo, unsigned_int_type, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS)

std::vector<size_t>
clx::device::max_work_item_sizes() const {
    size_t actual_size = 0;
    std::vector<size_t> value(3, size_t(0));
    int_type ret;
    bool success = false;
    while (!success) {
        ret = ::clGetDeviceInfo(
            this->_ptr,
            CL_DEVICE_MAX_WORK_ITEM_SIZES,
            value.size()*sizeof(size_t),
            value.data(),
            &actual_size
        );
        value.resize(actual_size/sizeof(size_t));
        if (errc(ret) != errc::invalid_value && actual_size <= value.size()) {
            CLX_CHECK(ret);
            success = true;
        }
    }
    return value;
}

#if CL_TARGET_VERSION >= 210
CLX_METHOD_SCALAR(
    clx::device::max_sub_groups,
    ::clGetDeviceInfo,
    unsigned_int_type,
    CL_DEVICE_MAX_NUM_SUB_GROUPS
);
CLX_METHOD_BOOL(
    clx::device::sub_groups_have_independent_progress,
    ::clGetDeviceInfo,
    CL_DEVICE_SUB_GROUP_INDEPENDENT_FORWARD_PROGRESS
);
#endif

#if CL_TARGET_VERSION >= 200
CLX_METHOD_SCALAR(
    clx::device::pipe_max_reservations,
    ::clGetDeviceInfo,
    unsigned_int_type,
    CL_DEVICE_PIPE_MAX_ACTIVE_RESERVATIONS
);
CLX_METHOD_SCALAR(
    clx::device::pipe_max_packet_size,
    ::clGetDeviceInfo,
    unsigned_int_type,
    CL_DEVICE_PIPE_MAX_PACKET_SIZE
);
#endif

CLX_METHOD_SCALAR(
    clx::device::host_queue_supported_flags,
    ::clGetDeviceInfo,
    command_queue_flags,
    #if CL_TARGET_VERSION >= 200
    CL_DEVICE_QUEUE_ON_HOST_PROPERTIES
    #else
    CL_DEVICE_QUEUE_PROPERTIES
    #endif
);

#if CL_TARGET_VERSION >= 200
CLX_METHOD_SCALAR(
    clx::device::device_queue_supported_flags,
    ::clGetDeviceInfo,
    command_queue_flags,
    CL_DEVICE_QUEUE_ON_DEVICE_PROPERTIES
);
CLX_METHOD_SCALAR(
    clx::device::device_queue_max_size,
    ::clGetDeviceInfo,
    unsigned_int_type,
    CL_DEVICE_QUEUE_ON_DEVICE_MAX_SIZE
);
CLX_METHOD_SCALAR(
    clx::device::device_queue_preferred_size,
    ::clGetDeviceInfo,
    unsigned_int_type,
    CL_DEVICE_QUEUE_ON_DEVICE_PREFERRED_SIZE
);
CLX_METHOD_SCALAR(
    clx::device::device_queue_max_events,
    ::clGetDeviceInfo,
    unsigned_int_type,
    CL_DEVICE_MAX_ON_DEVICE_EVENTS
);
CLX_METHOD_SCALAR(
    clx::device::max_device_queues,
    ::clGetDeviceInfo,
    unsigned_int_type,
    CL_DEVICE_MAX_ON_DEVICE_QUEUES
);
#endif

#if CL_TARGET_VERSION >= 200
CLX_METHOD_SCALAR(
    clx::device::preferred_platform_atomic_alignment,
    ::clGetDeviceInfo,
    unsigned_int_type,
    CL_DEVICE_PREFERRED_PLATFORM_ATOMIC_ALIGNMENT
);
CLX_METHOD_SCALAR(
    clx::device::preferred_global_atomic_alignment,
    ::clGetDeviceInfo,
    unsigned_int_type,
    CL_DEVICE_PREFERRED_GLOBAL_ATOMIC_ALIGNMENT
);
CLX_METHOD_SCALAR(
    clx::device::preferred_local_atomic_alignment,
    ::clGetDeviceInfo,
    unsigned_int_type,
    CL_DEVICE_PREFERRED_LOCAL_ATOMIC_ALIGNMENT
);
#endif

#if CL_TARGET_VERSION >= 200
CLX_METHOD_SCALAR(
    clx::device::supported_svm_flags_200,
    ::clGetDeviceInfo,
    svm_flags,
    CL_DEVICE_SVM_CAPABILITIES
);
#elif CL_TARGET_VERSION >= 120 && defined(cl_arm_shared_virtual_memory)
CLX_METHOD_SCALAR(
    clx::device::supported_svm_flags_arm,
    ::clGetDeviceInfo,
    svm_flags,
    CL_DEVICE_SVM_CAPABILITIES_ARM
);
#endif



CLX_METHOD_SCALAR(clx::device::type, ::clGetDeviceInfo, device_flags, CL_DEVICE_TYPE);

CLX_METHOD_SCALAR(
    clx::device::compute_capability_major,
    ::clGetDeviceInfo,
    unsigned_int_type,
    CL_DEVICE_COMPUTE_CAPABILITY_MAJOR_NV
);

CLX_METHOD_SCALAR(
    clx::device::compute_capability_minor,
    ::clGetDeviceInfo,
    unsigned_int_type,
    CL_DEVICE_COMPUTE_CAPABILITY_MINOR_NV
);

CLX_METHOD_SCALAR(
    clx::device::max_registers,
    ::clGetDeviceInfo,
    unsigned_int_type,
    CL_DEVICE_REGISTERS_PER_BLOCK_NV
);

CLX_METHOD_SCALAR(
    clx::device::warp_size,
    ::clGetDeviceInfo,
    unsigned_int_type,
    CL_DEVICE_WARP_SIZE_NV
);

CLX_METHOD_BOOL(
    clx::device::overlap,
    ::clGetDeviceInfo,
    CL_DEVICE_GPU_OVERLAP_NV
);

CLX_METHOD_BOOL(
    clx::device::kernel_execution_timeout,
    ::clGetDeviceInfo,
    CL_DEVICE_KERNEL_EXEC_TIMEOUT_NV
);

CLX_METHOD_BOOL(
    clx::device::integrated_memory,
    ::clGetDeviceInfo,
    CL_DEVICE_INTEGRATED_MEMORY_NV
);

CLX_METHOD_STRING(clx::device::board_name, ::clGetDeviceInfo, CL_DEVICE_BOARD_NAME_AMD);

CLX_METHOD_SCALAR(
    clx::device::simd_per_compute_unit,
    ::clGetDeviceInfo,
    unsigned_int_type,
    CL_DEVICE_SIMD_PER_COMPUTE_UNIT_AMD
);

CLX_METHOD_SCALAR(
    clx::device::simd_width,
    ::clGetDeviceInfo,
    unsigned_int_type,
    CL_DEVICE_SIMD_WIDTH_AMD
);

CLX_METHOD_SCALAR(
    clx::device::simd_instruction_width,
    ::clGetDeviceInfo,
    unsigned_int_type,
    CL_DEVICE_SIMD_INSTRUCTION_WIDTH_AMD
);

CLX_METHOD_SCALAR(
    clx::device::wavefront_width,
    ::clGetDeviceInfo,
    unsigned_int_type,
    CL_DEVICE_WAVEFRONT_WIDTH_AMD
);

CLX_METHOD_SCALAR(
    clx::device::num_global_memory_channels,
    ::clGetDeviceInfo,
    unsigned_int_type,
    CL_DEVICE_GLOBAL_MEM_CHANNELS_AMD
);

CLX_METHOD_SCALAR(
    clx::device::num_global_memory_channel_banks,
    ::clGetDeviceInfo,
    unsigned_int_type,
    CL_DEVICE_GLOBAL_MEM_CHANNEL_BANKS_AMD
);

CLX_METHOD_SCALAR(
    clx::device::num_global_memory_channel_bank_width,
    ::clGetDeviceInfo,
    unsigned_int_type,
    CL_DEVICE_GLOBAL_MEM_CHANNEL_BANK_WIDTH_AMD
);

CLX_METHOD_SCALAR(
    clx::device::local_memory_size_per_compute_unit,
    ::clGetDeviceInfo,
    unsigned_int_type,
    CL_DEVICE_LOCAL_MEM_SIZE_PER_COMPUTE_UNIT_AMD
);

CLX_METHOD_SCALAR(
    clx::device::num_local_memory_banks,
    ::clGetDeviceInfo,
    unsigned_int_type,
    CL_DEVICE_LOCAL_MEM_BANKS_AMD
);

CLX_METHOD_BOOL(
    clx::device::supports_thread_trace,
    ::clGetDeviceInfo,
    CL_DEVICE_THREAD_TRACE_SUPPORTED_AMD
);

CLX_METHOD_SCALAR(
    clx::device::gfxip_major,
    ::clGetDeviceInfo,
    unsigned_int_type,
    CL_DEVICE_GFXIP_MAJOR_AMD
);

CLX_METHOD_SCALAR(
    clx::device::gfxip_minor,
    ::clGetDeviceInfo,
    unsigned_int_type,
    CL_DEVICE_GFXIP_MINOR_AMD
);

CLX_METHOD_SCALAR(
    clx::device::num_asynchronous_queues,
    ::clGetDeviceInfo,
    unsigned_int_type,
    CL_DEVICE_AVAILABLE_ASYNC_QUEUES_AMD
);

CLX_METHOD_SCALAR(
    clx::device::preferred_work_group_size,
    ::clGetDeviceInfo,
    size_t,
    CL_DEVICE_PREFERRED_WORK_GROUP_SIZE_AMD
);

CLX_METHOD_SCALAR(
    clx::device::preferred_constant_buffer_size,
    ::clGetDeviceInfo,
    unsigned_long_type,
    CL_DEVICE_PREFERRED_CONSTANT_BUFFER_SIZE_AMD
);

CLX_METHOD_SCALAR(
    clx::device::pcie_id,
    ::clGetDeviceInfo,
    unsigned_int_type,
    CL_DEVICE_PCIE_ID_AMD
);

CLX_METHOD_ARRAY(
    clx::device::global_free_memory,
    ::clGetDeviceInfo,
    CL_DEVICE_GLOBAL_FREE_MEMORY_AMD,
    size_t
)


#if defined(CL_DEVICE_MAX_NAMED_BARRIER_COUNT_KHR)
CLX_METHOD_SCALAR(
    clx::device::max_named_barriers,
    ::clGetDeviceInfo,
    unsigned_int_type,
    CL_DEVICE_MAX_NAMED_BARRIER_COUNT_KHR
)
#endif

#if defined(cl_arm_core_id) && CL_TARGET_VERSION >= 120
CLX_METHOD_SCALAR(
    clx::device::compute_units_mask,
    ::clGetDeviceInfo,
    unsigned_long_type,
    CL_DEVICE_COMPUTE_UNITS_BITFIELD_ARM
);
#endif

#if CL_TARGET_VERSION >= 110
CLX_METHOD_BOOL(
    clx::device::has_unified_memory,
    ::clGetDeviceInfo,
    CL_DEVICE_HOST_UNIFIED_MEMORY
);
#endif

#if defined(CL_DEVICE_CORE_TEMPERATURE_ALTERA)
CLX_METHOD_SCALAR(
    clx::device::core_temperature,
    ::clGetDeviceInfo,
    int_type,
    CL_DEVICE_CORE_TEMPERATURE_ALTERA
);
#endif

#if defined(CL_DEVICE_MAX_ATOMIC_COUNTERS_EXT)
CLX_METHOD_SCALAR(
    clx::device::max_atomic_counters,
    ::clGetDeviceInfo,
    unsigned_int_type,
    CL_DEVICE_MAX_ATOMIC_COUNTERS_EXT
);
#endif

#if CL_TARGET_VERSION >= 120
CLX_METHOD_SCALAR(
    clx::device::parent,
    ::clGetDeviceInfo,
    device,
    CL_DEVICE_PARENT_DEVICE
);

CLX_METHOD_SCALAR(
    clx::device::max_subordinate_devices,
    ::clGetDeviceInfo,
    unsigned_int_type,
    CL_DEVICE_PARTITION_MAX_SUB_DEVICES
);

CLX_METHOD_SCALAR(
    clx::device::affinity_domain,
    ::clGetDeviceInfo,
    device_affinity_domain,
    CL_DEVICE_PARTITION_AFFINITY_DOMAIN
);

CLX_METHOD_ARRAY(
    clx::device::supported_partitions_priv,
    ::clGetDeviceInfo,
    CL_DEVICE_PARTITION_PROPERTIES,
    device_partition
);

CLX_METHOD_ARRAY(
    clx::device::partitions,
    ::clGetDeviceInfo,
    CL_DEVICE_PARTITION_TYPE,
    device_partition
);


std::vector<clx::device_partition>
clx::device::supported_partitions() const {
    auto partitions = this->supported_partitions_priv();
    if (!partitions.empty() && partitions.front() == device_partition{}) {
        partitions.clear();
    }
    return partitions;
}

#endif

#if CL_TARGET_VERSION >= 120
std::vector<clx::device>
clx::device::partition(unsigned int num_compute_units) const {
    std::vector<partition_property_type> properties{
        partition_property_type(device_partition::equally),
        partition_property_type(num_compute_units),
        partition_property_type(0)
    };
    std::vector<device> result(4096 / sizeof(device));
    unsigned_int_type ndevices = 0;
    int_type ret;
    bool success = false;
    while (!success) {
        ret = ::clCreateSubDevices(
            this->_ptr, properties.data(),
            result.size(), downcast(result.data()),
            &ndevices
        );
        result.resize(ndevices);
        if ((errc(ret) != errc::invalid_value &&
            ndevices <= result.size()) || ndevices == 0) {
            CLX_CHECK(ret);
            success = true;
        }
    }
    result.resize(ndevices);
    return result;
}
#endif

#if CL_TARGET_VERSION >= 120
std::vector<clx::device>
clx::device::partition(const std::vector<unsigned int>& num_compute_units) const {
    std::vector<partition_property_type> properties;
    properties.reserve(2 + num_compute_units.size());
    properties.emplace_back(
        static_cast<partition_property_type>(device_partition::by_counts)
    );
    for (const auto& n : num_compute_units) {
        properties.emplace_back(n);
    }
    properties.emplace_back(
        static_cast<partition_property_type>(device_partition::by_counts_end)
    );
    properties.emplace_back(0);
    std::vector<device> result(num_compute_units.size());
    unsigned_int_type actual_size = 0;
    CLX_CHECK(::clCreateSubDevices(
        this->_ptr, properties.data(),
        result.size(), downcast(result.data()),
        &actual_size
    ));
    result.resize(actual_size);
    return result;
}
#endif

#if CL_TARGET_VERSION >= 110 && defined(cl_ext_device_fission)
std::vector<clx::ext::device>
clx::device::partition(const ext::partition_properties& properties) const {
    auto func = CLX_EXTENSION(clCreateSubDevicesEXT, platform());
    std::vector<ext::device> result(4096 / sizeof(device));
    unsigned_int_type ndevices = 0;
    int_type ret;
    bool success = false;
    while (!success) {
        ret = func(
            this->_ptr, properties.data(),
            result.size(), downcast(result.data()),
            &ndevices
        );
        result.resize(ndevices);
        if ((errc(ret) != errc::invalid_value &&
            ndevices <= result.size()) || ndevices == 0) {
            CLX_CHECK(ret);
            success = true;
        }
    }
    result.resize(ndevices);
    return result;
}
#endif
