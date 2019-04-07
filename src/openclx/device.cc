#include <openclx/bits/macros>
#include <openclx/command_queue>
#include <openclx/context>
#include <openclx/device>
#include <openclx/downcast>
#include <openclx/platform>

CLX_WARNING_PUSH
CLX_IGNORED_ATTRIBUTES

#define CLX_DEVICE_PREFERRED_VECTOR_WIDTH(type, type2) \
	template <> \
	clx::unsigned_int_type \
	clx::device::preferred_vector_width<clx::type>() const { \
		CLX_BODY_SCALAR(::clGetDeviceInfo, unsigned_int_type, \
			CL_DEVICE_PREFERRED_VECTOR_WIDTH_##type2) \
	}

CLX_METHOD_STRING(clx::device::name, ::clGetDeviceInfo, CL_DEVICE_NAME)
CLX_METHOD_STRING(clx::device::profile, ::clGetDeviceInfo, CL_DEVICE_PROFILE)
CLX_METHOD_STRING(clx::device::vendor, ::clGetDeviceInfo, CL_DEVICE_VENDOR)
CLX_METHOD_STRING(clx::device::version, ::clGetDeviceInfo, CL_DEVICE_VERSION)
CLX_METHOD_STRING(clx::device::driver_version, ::clGetDeviceInfo, CL_DRIVER_VERSION)
CLX_METHOD_STRING(clx::device::extensions, ::clGetDeviceInfo, CL_DEVICE_EXTENSIONS)

#if defined(CL_DEVICE_SPIR_VERSIONS)
CLX_METHOD_STRING(
	clx::device::spir_versions,
	::clGetDeviceInfo,
	CL_DEVICE_SPIR_VERSIONS
)
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
)


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
	queue_properties_type props[] = {
		queue_properties_type(CL_QUEUE_PROPERTIES),
		queue_properties_type(flags),
		queue_properties_type(0)
	};
	int_type ret = 0;
	auto result =
		::clCreateCommandQueueWithProperties(ctx, this->_ptr, props, &ret);
	CLX_CHECK(ret);
	return static_cast<::clx::command_queue>(result);
}
#endif

#if CL_TARGET_VERSION >= 200
clx::command_queue
clx::device::queue_200(
	context_type ctx,
	command_queue_flags flags,
	unsigned_int_type size
) const {
	queue_properties_type props[] = {
		queue_properties_type(CL_QUEUE_PROPERTIES),
		queue_properties_type(flags),
		queue_properties_type(CL_QUEUE_SIZE),
		queue_properties_type(size),
		queue_properties_type(0)
	};
	int_type ret = 0;
	auto result =
		::clCreateCommandQueueWithProperties(ctx, this->_ptr, props, &ret);
	CLX_CHECK(ret);
	return static_cast<::clx::command_queue>(result);
}
#endif

#if CL_TARGET_VERSION <= 120 || defined(CL_USE_DEPRECATED_OPENCL_1_2_APIS)
clx::command_queue
clx::device::queue_100(context_type ctx, command_queue_flags flags) const {
	int_type ret = 0;
	auto result = ::clCreateCommandQueue(ctx, this->_ptr, downcast(flags), &ret);
	CLX_CHECK(ret);
	return static_cast<::clx::command_queue>(result);
}
#endif

#if CL_TARGET_VERSION <= 120 || defined(CL_USE_DEPRECATED_OPENCL_1_2_APIS)
clx::command_queue
clx::device::queue_100(
	context_type ctx,
	command_queue_flags flags,
	unsigned_int_type size
) const {
	queue_properties_type props[] = {
		queue_properties_type(CL_QUEUE_PROPERTIES),
		queue_properties_type(flags),
		queue_properties_type(CL_QUEUE_SIZE),
		queue_properties_type(size),
		queue_properties_type(0)
	};
	int_type ret = 0;
	auto func = CLX_EXTENSION(clCreateCommandQueueWithPropertiesKHR, platform());
	if (!func) {
		throw std::runtime_error("cl_khr_create_command_queue is not supported");
	}
	auto result = func(ctx, this->_ptr, props, &ret);
	CLX_CHECK(ret);
	return static_cast<::clx::command_queue>(result);
}
#endif

CLX_METHOD_BOOL(clx::device::available, ::clGetDeviceInfo, CL_DEVICE_AVAILABLE)
CLX_METHOD_BOOL(clx::device::compiler_available, ::clGetDeviceInfo, CL_DEVICE_COMPILER_AVAILABLE)
CLX_METHOD_BOOL(clx::device::little_endian, ::clGetDeviceInfo, CL_DEVICE_ENDIAN_LITTLE)
CLX_METHOD_BOOL(clx::device::supports_error_correction, ::clGetDeviceInfo, CL_DEVICE_ERROR_CORRECTION_SUPPORT)
CLX_METHOD_BOOL(clx::device::supports_images, ::clGetDeviceInfo, CL_DEVICE_IMAGE_SUPPORT)

CLX_METHOD_SCALAR(clx::device::image2d_max_width, ::clGetDeviceInfo, size_t, CL_DEVICE_IMAGE2D_MAX_WIDTH)
CLX_METHOD_SCALAR(clx::device::image2d_max_height, ::clGetDeviceInfo, size_t, CL_DEVICE_IMAGE2D_MAX_HEIGHT)
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

CLX_DEVICE_PREFERRED_VECTOR_WIDTH(char_type, CHAR)
CLX_DEVICE_PREFERRED_VECTOR_WIDTH(short_type, SHORT)
CLX_DEVICE_PREFERRED_VECTOR_WIDTH(int_type, INT)
CLX_DEVICE_PREFERRED_VECTOR_WIDTH(long_type, LONG)
CLX_DEVICE_PREFERRED_VECTOR_WIDTH(float_type, FLOAT)
CLX_DEVICE_PREFERRED_VECTOR_WIDTH(double_type, DOUBLE)

CLX_METHOD_SCALAR(clx::device::max_clock_frequency, ::clGetDeviceInfo, unsigned_int_type, CL_DEVICE_MAX_CLOCK_FREQUENCY)
CLX_METHOD_SCALAR(clx::device::max_compute_units, ::clGetDeviceInfo, unsigned_int_type, CL_DEVICE_MAX_COMPUTE_UNITS)

CLX_METHOD_SCALAR(clx::device::max_constant_arguments, ::clGetDeviceInfo, unsigned_int_type, CL_DEVICE_MAX_CONSTANT_ARGS)
CLX_METHOD_SCALAR(clx::device::max_read_image_arguments, ::clGetDeviceInfo, unsigned_int_type, CL_DEVICE_MAX_READ_IMAGE_ARGS)
CLX_METHOD_SCALAR(clx::device::max_write_image_arguments, ::clGetDeviceInfo, unsigned_int_type, CL_DEVICE_MAX_WRITE_IMAGE_ARGS)
CLX_METHOD_SCALAR(clx::device::max_samplers, ::clGetDeviceInfo, unsigned_int_type, CL_DEVICE_MAX_SAMPLERS)
CLX_METHOD_SCALAR(clx::device::max_parameter_size, ::clGetDeviceInfo, size_t, CL_DEVICE_MAX_PARAMETER_SIZE)

CLX_METHOD_SCALAR(clx::device::max_constant_buffer_size, ::clGetDeviceInfo, unsigned_long_type, CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE)
CLX_METHOD_SCALAR(clx::device::max_memory_allocation_size, ::clGetDeviceInfo, unsigned_long_type, CL_DEVICE_MAX_MEM_ALLOC_SIZE)

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

CLX_METHOD_SCALAR(clx::device::queue_properties, ::clGetDeviceInfo, command_queue_flags, CL_DEVICE_QUEUE_PROPERTIES)
CLX_METHOD_SCALAR(clx::device::type, ::clGetDeviceInfo, device_flags, CL_DEVICE_TYPE)

#if CL_TARGET_VERSION >= 120
CLX_METHOD_SCALAR(clx::device::parent, ::clGetDeviceInfo, device, CL_DEVICE_PARENT_DEVICE)
CLX_METHOD_SCALAR(clx::device::max_subordinate_devices, ::clGetDeviceInfo, unsigned_int_type, CL_DEVICE_PARTITION_MAX_SUB_DEVICES)
CLX_METHOD_SCALAR(clx::device::affinity_domain, ::clGetDeviceInfo, device_affinity_domain, CL_DEVICE_PARTITION_AFFINITY_DOMAIN)
CLX_METHOD_ARRAY(clx::device::supported_partitions_priv, ::clGetDeviceInfo, CL_DEVICE_PARTITION_PROPERTIES, device_partition)
CLX_METHOD_ARRAY(clx::device::partitions, ::clGetDeviceInfo, CL_DEVICE_PARTITION_TYPE, device_partition)


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
	std::vector<device_partition_type> properties{
		device_partition_type(device_partition::equally),
		device_partition_type(num_compute_units),
		device_partition_type(0)
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
		this->_ptr, properties.data(),
		result.size(), downcast(result.data()),
		&actual_size
	));
	result.resize(actual_size);
	return result;
}
#endif

CLX_WARNING_POP
