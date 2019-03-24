#include <openclx/bits/macros>
#include <openclx/command_queue>
#include <openclx/context>
#include <openclx/device>
#include <openclx/platform>

CLX_WARNING_PUSH
CLX_IGNORED_ATTRIBUTES

#define CLX_DEVICE_PREFERRED_VECTOR_WIDTH(type, type2) \
	template <> \
	clx::unsigned_int_type \
	clx::device::preferred_vector_width<clx::type>() const { \
		CLX_BODY_SCALAR(::clGetDeviceInfo, \
			CL_DEVICE_PREFERRED_VECTOR_WIDTH_##type2, unsigned_int_type) \
	}

CLX_METHOD_STRING(clx::device::name, ::clGetDeviceInfo, CL_DEVICE_NAME)
CLX_METHOD_STRING(clx::device::profile, ::clGetDeviceInfo, CL_DEVICE_PROFILE)
CLX_METHOD_STRING(clx::device::vendor, ::clGetDeviceInfo, CL_DEVICE_VENDOR)
CLX_METHOD_STRING(clx::device::version, ::clGetDeviceInfo, CL_DEVICE_VERSION)
CLX_METHOD_STRING(clx::device::driver_version, ::clGetDeviceInfo, CL_DRIVER_VERSION)
CLX_METHOD_STRING(clx::device::extensions, ::clGetDeviceInfo, CL_DEVICE_EXTENSIONS)

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

CLX_METHOD_SCALAR(clx::device::timer_resolution, ::clGetDeviceInfo, CL_DEVICE_PROFILING_TIMER_RESOLUTION, nanoseconds)

CLX_METHOD_SCALAR(clx::device::vendor_id, ::clGetDeviceInfo, CL_DEVICE_VENDOR_ID, unsigned_int_type)
CLX_METHOD_SCALAR(clx::device::platform, ::clGetDeviceInfo, CL_DEVICE_PLATFORM, ::clx::platform)


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
clx::device::queue_200(context_type ctx, command_queue_flags flags) const {
	std::vector<queue_properties_type> props{
		queue_properties_type(CL_QUEUE_PROPERTIES),
		queue_properties_type(flags),
		queue_properties_type(0)
	};
	int_type ret = 0;
	auto result =
		::clCreateCommandQueueWithProperties(
			ctx, this->_ptr, props.data(), &ret
		);
	CLX_CHECK(ret);
	return static_cast<::clx::command_queue>(result);
}
#endif

#if CL_TARGET_VERSION <= 120 || defined(CL_USE_DEPRECATED_OPENCL_1_2_APIS)
clx::command_queue
clx::device::queue_100(context_type ctx, command_queue_flags flags) const {
	int_type ret = 0;
	auto result = ::clCreateCommandQueue(
		ctx, this->_ptr, static_cast<command_queue_flags_type>(flags), &ret
	);
	CLX_CHECK(ret);
	return static_cast<::clx::command_queue>(result);
}
#endif

CLX_METHOD_BOOL(clx::device::available, ::clGetDeviceInfo, CL_DEVICE_AVAILABLE)
CLX_METHOD_BOOL(clx::device::compiler_available, ::clGetDeviceInfo, CL_DEVICE_COMPILER_AVAILABLE)
CLX_METHOD_BOOL(clx::device::little_endian, ::clGetDeviceInfo, CL_DEVICE_ENDIAN_LITTLE)
CLX_METHOD_BOOL(clx::device::supports_error_correction, ::clGetDeviceInfo, CL_DEVICE_ERROR_CORRECTION_SUPPORT)
CLX_METHOD_BOOL(clx::device::supports_images, ::clGetDeviceInfo, CL_DEVICE_IMAGE_SUPPORT)

CLX_METHOD_SCALAR(clx::device::image2d_max_width, ::clGetDeviceInfo, CL_DEVICE_IMAGE2D_MAX_WIDTH, size_t)
CLX_METHOD_SCALAR(clx::device::image2d_max_height, ::clGetDeviceInfo, CL_DEVICE_IMAGE2D_MAX_HEIGHT, size_t)
CLX_METHOD_SCALAR(clx::device::image3d_max_width, ::clGetDeviceInfo, CL_DEVICE_IMAGE3D_MAX_WIDTH, size_t)
CLX_METHOD_SCALAR(clx::device::image3d_max_height, ::clGetDeviceInfo, CL_DEVICE_IMAGE3D_MAX_HEIGHT, size_t)
CLX_METHOD_SCALAR(clx::device::image3d_max_depth, ::clGetDeviceInfo, CL_DEVICE_IMAGE3D_MAX_DEPTH, size_t)

CLX_METHOD_SCALAR(clx::device::global_memory_size, ::clGetDeviceInfo, CL_DEVICE_GLOBAL_MEM_SIZE, unsigned_long_type)
CLX_METHOD_SCALAR(clx::device::global_memory_cache_size, ::clGetDeviceInfo, CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, unsigned_long_type)
CLX_METHOD_SCALAR(clx::device::global_memory_cacheline_size, ::clGetDeviceInfo, CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE, unsigned_int_type)
CLX_METHOD_SCALAR(clx::device::global_memory_cache_type, ::clGetDeviceInfo, CL_DEVICE_GLOBAL_MEM_CACHE_TYPE, device_memory_cache)
CLX_METHOD_SCALAR(clx::device::local_memory_size, ::clGetDeviceInfo, CL_DEVICE_LOCAL_MEM_SIZE, unsigned_long_type)
CLX_METHOD_SCALAR(clx::device::local_memory_type, ::clGetDeviceInfo, CL_DEVICE_LOCAL_MEM_TYPE, device_local_memory)

CLX_METHOD_SCALAR(clx::device::address_bits, ::clGetDeviceInfo, CL_DEVICE_ADDRESS_BITS, unsigned_int_type)
CLX_METHOD_SCALAR(clx::device::base_address_alignment, ::clGetDeviceInfo, CL_DEVICE_MEM_BASE_ADDR_ALIGN, unsigned_int_type)
CLX_METHOD_SCALAR(clx::device::min_data_alignment, ::clGetDeviceInfo, CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE, unsigned_int_type)

CLX_METHOD_SCALAR(clx::device::execution_capabilities, ::clGetDeviceInfo, CL_DEVICE_EXECUTION_CAPABILITIES, execution_flags)

template <>
clx::floating_point_flags
clx::device::floating_point_capabilities<clx::half_type>() const {
	CLX_BODY_SCALAR(::clGetDeviceInfo, CL_DEVICE_HALF_FP_CONFIG, floating_point_flags)
}

template <>
clx::floating_point_flags
clx::device::floating_point_capabilities<clx::float_type>() const {
	CLX_BODY_SCALAR(::clGetDeviceInfo, CL_DEVICE_SINGLE_FP_CONFIG, floating_point_flags)
}

template <>
clx::floating_point_flags
clx::device::floating_point_capabilities<clx::double_type>() const {
	CLX_BODY_SCALAR(::clGetDeviceInfo, CL_DEVICE_DOUBLE_FP_CONFIG, floating_point_flags)
}

CLX_DEVICE_PREFERRED_VECTOR_WIDTH(char_type, CHAR)
CLX_DEVICE_PREFERRED_VECTOR_WIDTH(short_type, SHORT)
CLX_DEVICE_PREFERRED_VECTOR_WIDTH(int_type, INT)
CLX_DEVICE_PREFERRED_VECTOR_WIDTH(long_type, LONG)
CLX_DEVICE_PREFERRED_VECTOR_WIDTH(float_type, FLOAT)
CLX_DEVICE_PREFERRED_VECTOR_WIDTH(double_type, DOUBLE)

CLX_METHOD_SCALAR(clx::device::max_clock_frequency, ::clGetDeviceInfo, CL_DEVICE_MAX_CLOCK_FREQUENCY, unsigned_int_type)
CLX_METHOD_SCALAR(clx::device::max_compute_units, ::clGetDeviceInfo, CL_DEVICE_MAX_COMPUTE_UNITS, unsigned_int_type)

CLX_METHOD_SCALAR(clx::device::max_constant_arguments, ::clGetDeviceInfo, CL_DEVICE_MAX_CONSTANT_ARGS, unsigned_int_type)
CLX_METHOD_SCALAR(clx::device::max_read_image_arguments, ::clGetDeviceInfo, CL_DEVICE_MAX_READ_IMAGE_ARGS, unsigned_int_type)
CLX_METHOD_SCALAR(clx::device::max_write_image_arguments, ::clGetDeviceInfo, CL_DEVICE_MAX_WRITE_IMAGE_ARGS, unsigned_int_type)
CLX_METHOD_SCALAR(clx::device::max_samplers, ::clGetDeviceInfo, CL_DEVICE_MAX_SAMPLERS, unsigned_int_type)
CLX_METHOD_SCALAR(clx::device::max_parameter_size, ::clGetDeviceInfo, CL_DEVICE_MAX_PARAMETER_SIZE, size_t)

CLX_METHOD_SCALAR(clx::device::max_constant_buffer_size, ::clGetDeviceInfo, CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, unsigned_long_type)
CLX_METHOD_SCALAR(clx::device::max_memory_allocation_size, ::clGetDeviceInfo, CL_DEVICE_MAX_MEM_ALLOC_SIZE, unsigned_long_type)

CLX_METHOD_SCALAR(clx::device::max_work_group_size, ::clGetDeviceInfo, CL_DEVICE_MAX_WORK_GROUP_SIZE, size_t)
CLX_METHOD_SCALAR(clx::device::max_work_item_dimensions, ::clGetDeviceInfo, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, unsigned_int_type)

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

CLX_METHOD_SCALAR(clx::device::queue_properties, ::clGetDeviceInfo, CL_DEVICE_QUEUE_PROPERTIES, command_queue_flags)
CLX_METHOD_SCALAR(clx::device::type, ::clGetDeviceInfo, CL_DEVICE_TYPE, device_flags)

#if CL_TARGET_VERSION >= 120
CLX_METHOD_SCALAR(clx::device::parent, ::clGetDeviceInfo, CL_DEVICE_PARENT_DEVICE, device)
CLX_METHOD_SCALAR(clx::device::max_subordinate_devices, ::clGetDeviceInfo, CL_DEVICE_PARTITION_MAX_SUB_DEVICES, unsigned_int_type)
CLX_METHOD_SCALAR(clx::device::affinity_domain, ::clGetDeviceInfo, CL_DEVICE_PARTITION_AFFINITY_DOMAIN, device_affinity_domain)
CLX_METHOD_ARRAY(clx::device::supported_partitions, ::clGetDeviceInfo, CL_DEVICE_PARTITION_PROPERTIES, device_partition)
CLX_METHOD_ARRAY(clx::device::partitions, ::clGetDeviceInfo, CL_DEVICE_PARTITION_TYPE, device_partition)
#endif

#if CL_TARGET_VERSION >= 120
std::vector<clx::device>
clx::device::partition(unsigned int num_compute_units) const {
	std::vector<device_partition_type> properties{
		device_partition_type(device_partition::equally),
		device_partition_type(num_compute_units),
		device_partition_type(0)
	};
	std::vector<device> result(4096 / sizeof(device));
	unsigned_int_type actual_size = 0;
	int_type ret;
	bool success = false;
	while (!success) {
		ret = clCreateSubDevices(
			this->_ptr,
			properties.data(),
			result.size(),
			reinterpret_cast<device_type*>(result.data()),
			&actual_size
		);
		result.resize(actual_size/sizeof(size_t));
		if (errc(ret) != errc::invalid_value && actual_size <= result.size()) {
			CLX_CHECK(ret);
			success = true;
		}
	}
	result.resize(actual_size);
	return result;
}
#endif

#if CL_TARGET_VERSION >= 120
std::vector<clx::device>
clx::device::partition(const std::vector<unsigned int>& num_compute_units) const {
	std::vector<device_partition_type> properties;
	properties.reserve(2 + num_compute_units.size());
	properties.emplace_back(
		static_cast<device_partition_type>(device_partition::by_counts)
	);
	for (const auto& n : num_compute_units) {
		properties.emplace_back(n);
	}
	properties.emplace_back(
		static_cast<device_partition_type>(device_partition::by_counts_end)
	);
	properties.emplace_back(0);
	std::vector<device> result(num_compute_units.size());
	unsigned_int_type actual_size = 0;
	CLX_CHECK(::clCreateSubDevices(
		this->_ptr,
		properties.data(),
		result.size(),
		reinterpret_cast<device_type*>(result.data()),
		&actual_size
	));
	result.resize(actual_size);
	return result;
}
#endif

CLX_WARNING_POP
