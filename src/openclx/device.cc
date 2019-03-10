#include <openclx/bits/macros>
#include <openclx/command_queue>
#include <openclx/context>
#include <openclx/device>
#include <openclx/platform>

CLX_WARNING_PUSH
CLX_IGNORED_ATTRIBUTES

#define CLX_DEVICE_GET_STRING(name, key) \
	std::string \
	clx::device::name() const { \
		CLX_GET_STRING(::clGetDeviceInfo, key); \
	}

#define CLX_DEVICE_GET_SCALAR3(name, key, type) \
	auto \
	clx::device::name() const -> type { \
		CLX_GET_SCALAR3(::clGetDeviceInfo, key, type) \
	}

#define CLX_DEVICE_GET_SCALAR4(name, key, type, return_type) \
	auto \
	clx::device::name() const -> return_type { \
		CLX_GET_SCALAR4(::clGetDeviceInfo, key, type, return_type) \
	}

#define CLX_DEVICE_PREFERRED_VECTOR_WIDTH(type, type2) \
	template <> \
	clx::unsigned_int_type \
	clx::device::preferred_vector_width<clx::type>() const { \
		CLX_GET_SCALAR3(::clGetDeviceInfo, type2, unsigned_int_type) \
	}

#define CLX_DEVICE_GET_ARRAY(name, key, type, return_type) \
	auto \
	clx::device::name() const -> std::vector<return_type> { \
		CLX_GET_ARRAY(::clGetDeviceInfo, key, type, return_type) \
	}

CLX_DEVICE_GET_STRING(name, CL_DEVICE_NAME)
CLX_DEVICE_GET_STRING(profile, CL_DEVICE_PROFILE)
CLX_DEVICE_GET_STRING(vendor, CL_DEVICE_VENDOR)
CLX_DEVICE_GET_STRING(version, CL_DEVICE_VERSION)
CLX_DEVICE_GET_STRING(driver_version, CL_DRIVER_VERSION)
CLX_DEVICE_GET_STRING(extensions, CL_DEVICE_EXTENSIONS)

#if CLX_OPENCL_VERSION >= 210
auto
clx::device::host_time() const -> nanoseconds {
	unsigned_long_type value = 0;
	CLX_CHECK(::clGetHostTimer(this->_ptr, &value));
	return static_cast<nanoseconds>(value);
}
#endif

#if CLX_OPENCL_VERSION >= 210
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

CLX_DEVICE_GET_SCALAR4(timer_resolution, CL_DEVICE_PROFILING_TIMER_RESOLUTION, size_t, nanoseconds)

CLX_DEVICE_GET_SCALAR3(vendor_id, CL_DEVICE_VENDOR_ID, unsigned_int_type)
CLX_DEVICE_GET_SCALAR4(platform, CL_DEVICE_PLATFORM, platform_type, ::clx::platform)


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

clx::command_queue clx::device::queue(
	command_queue_flags_type flags
) const {
	return this->queue(this->context().get(), flags);
}

clx::command_queue clx::device::queue(
	context_type ctx,
	command_queue_flags_type flags
) const {
	#if CLX_OPENCL_VERSION >= 200
	std::vector<queue_properties_type> props{
		queue_properties_type(CL_QUEUE_PROPERTIES),
		queue_properties_type(flags),
		queue_properties_type(0)
	};
	#endif
	int_type ret = 0;
	#if CLX_OPENCL_VERSION >= 200
	auto result =
		::clCreateCommandQueueWithProperties(ctx, this->_ptr, props.data(), &ret);
	#else
	auto result = ::clCreateCommandQueue(ctx, this->_ptr, flags, &ret);
	#endif
	CLX_CHECK(ret);
	return static_cast<::clx::command_queue>(result);
}

CLX_DEVICE_GET_SCALAR4(available, CL_DEVICE_AVAILABLE, cl_bool, bool)
CLX_DEVICE_GET_SCALAR4(compiler_available, CL_DEVICE_COMPILER_AVAILABLE, cl_bool, bool)
CLX_DEVICE_GET_SCALAR4(little_endian, CL_DEVICE_ENDIAN_LITTLE, cl_bool, bool)
CLX_DEVICE_GET_SCALAR4(supports_error_correction, CL_DEVICE_ERROR_CORRECTION_SUPPORT, cl_bool, bool)
CLX_DEVICE_GET_SCALAR4(supports_images, CL_DEVICE_IMAGE_SUPPORT, cl_bool, bool)

CLX_DEVICE_GET_SCALAR3(image2d_max_width, CL_DEVICE_IMAGE2D_MAX_WIDTH, size_t)
CLX_DEVICE_GET_SCALAR3(image2d_max_height, CL_DEVICE_IMAGE2D_MAX_HEIGHT, size_t)
CLX_DEVICE_GET_SCALAR3(image3d_max_width, CL_DEVICE_IMAGE3D_MAX_WIDTH, size_t)
CLX_DEVICE_GET_SCALAR3(image3d_max_height, CL_DEVICE_IMAGE3D_MAX_HEIGHT, size_t)
CLX_DEVICE_GET_SCALAR3(image3d_max_depth, CL_DEVICE_IMAGE3D_MAX_DEPTH, size_t)

CLX_DEVICE_GET_SCALAR3(global_memory_size, CL_DEVICE_GLOBAL_MEM_SIZE, unsigned_long_type)
CLX_DEVICE_GET_SCALAR3(global_memory_cache_size, CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, unsigned_long_type)
CLX_DEVICE_GET_SCALAR3(global_memory_cacheline_size, CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE, unsigned_int_type)
CLX_DEVICE_GET_SCALAR4(global_memory_cache_type, CL_DEVICE_GLOBAL_MEM_CACHE_TYPE, device_memory_cache_type, device_memory_cache)
CLX_DEVICE_GET_SCALAR3(local_memory_size, CL_DEVICE_LOCAL_MEM_SIZE, unsigned_long_type)
CLX_DEVICE_GET_SCALAR4(local_memory_type, CL_DEVICE_LOCAL_MEM_TYPE, device_local_memory_type, device_local_memory)

CLX_DEVICE_GET_SCALAR3(address_bits, CL_DEVICE_ADDRESS_BITS, unsigned_int_type)
CLX_DEVICE_GET_SCALAR3(base_address_alignment, CL_DEVICE_MEM_BASE_ADDR_ALIGN, unsigned_int_type)
CLX_DEVICE_GET_SCALAR3(min_data_alignment, CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE, unsigned_int_type)

CLX_DEVICE_GET_SCALAR3(execution_capabilities, CL_DEVICE_EXECUTION_CAPABILITIES, device_execution_capabilities_type)

template <>
clx::device_floating_point_flags_type
clx::device::floating_point_capabilities<clx::half_type>() const {
	CLX_GET_SCALAR3(::clGetDeviceInfo, CL_DEVICE_HALF_FP_CONFIG, device_floating_point_flags_type)
}

template <>
clx::device_floating_point_flags_type
clx::device::floating_point_capabilities<clx::float_type>() const {
	CLX_GET_SCALAR3(::clGetDeviceInfo, CL_DEVICE_SINGLE_FP_CONFIG, device_floating_point_flags_type)
}

template <>
clx::device_floating_point_flags_type
clx::device::floating_point_capabilities<clx::double_type>() const {
	CLX_GET_SCALAR3(::clGetDeviceInfo, CL_DEVICE_DOUBLE_FP_CONFIG, device_floating_point_flags_type)
}

CLX_DEVICE_PREFERRED_VECTOR_WIDTH(char_type, CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR)
CLX_DEVICE_PREFERRED_VECTOR_WIDTH(short_type, CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT)
CLX_DEVICE_PREFERRED_VECTOR_WIDTH(int_type, CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT)
CLX_DEVICE_PREFERRED_VECTOR_WIDTH(long_type, CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG)
CLX_DEVICE_PREFERRED_VECTOR_WIDTH(float_type, CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT)
CLX_DEVICE_PREFERRED_VECTOR_WIDTH(double_type, CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE)

CLX_DEVICE_GET_SCALAR3(max_clock_frequency, CL_DEVICE_MAX_CLOCK_FREQUENCY, unsigned_int_type)
CLX_DEVICE_GET_SCALAR3(max_compute_units, CL_DEVICE_MAX_COMPUTE_UNITS, unsigned_int_type)

CLX_DEVICE_GET_SCALAR3(max_constant_arguments, CL_DEVICE_MAX_CONSTANT_ARGS, unsigned_int_type)
CLX_DEVICE_GET_SCALAR3(max_read_image_arguments, CL_DEVICE_MAX_READ_IMAGE_ARGS, unsigned_int_type)
CLX_DEVICE_GET_SCALAR3(max_write_image_arguments, CL_DEVICE_MAX_WRITE_IMAGE_ARGS, unsigned_int_type)
CLX_DEVICE_GET_SCALAR3(max_samplers, CL_DEVICE_MAX_SAMPLERS, unsigned_int_type)
CLX_DEVICE_GET_SCALAR3(max_parameter_size, CL_DEVICE_MAX_PARAMETER_SIZE, size_t)

CLX_DEVICE_GET_SCALAR3(max_constant_buffer_size, CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, unsigned_long_type)
CLX_DEVICE_GET_SCALAR3(max_memory_allocation_size, CL_DEVICE_MAX_MEM_ALLOC_SIZE, unsigned_long_type)

CLX_DEVICE_GET_SCALAR3(max_work_group_size, CL_DEVICE_MAX_WORK_GROUP_SIZE, size_t)
CLX_DEVICE_GET_SCALAR3(max_work_item_dimensions, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, unsigned_int_type)

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

CLX_DEVICE_GET_SCALAR3(queue_properties, CL_DEVICE_QUEUE_PROPERTIES, command_queue_flags_type)
CLX_DEVICE_GET_SCALAR3(type, CL_DEVICE_TYPE, device_flags_type)

#if CLX_OPENCL_VERSION >= 120
CLX_DEVICE_GET_SCALAR4(parent, CL_DEVICE_PARENT_DEVICE, device_type, device)
CLX_DEVICE_GET_SCALAR3(max_subordinate_devices, CL_DEVICE_PARTITION_MAX_SUB_DEVICES, unsigned_int_type)
CLX_DEVICE_GET_SCALAR4(affinity_domain, CL_DEVICE_PARTITION_AFFINITY_DOMAIN, device_affinity_domain_type, device_affinity_domain)
CLX_DEVICE_GET_ARRAY(supported_partitions, CL_DEVICE_PARTITION_PROPERTIES, device_partition_type, device_partition)
CLX_DEVICE_GET_ARRAY(partitions, CL_DEVICE_PARTITION_TYPE, device_partition_type, device_partition)
#endif

#if CLX_OPENCL_VERSION >= 120
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

#if CLX_OPENCL_VERSION >= 120
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
