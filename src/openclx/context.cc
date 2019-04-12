#include <openclx/binary>
#include <openclx/bits/macros>
#include <openclx/bits/pragmas>
#include <openclx/buffer>
#include <openclx/command_queue>
#include <openclx/context>
#include <openclx/device>
#include <openclx/downcast>
#include <openclx/event>
#include <openclx/image>
#include <openclx/pipe>
#include <openclx/platform>
#include <openclx/program>
#include <openclx/sampler>
#include <openclx/sampler_properties>
#include <openclx/svm_block>

CLX_WARNING_PUSH
CLX_IGNORED_ATTRIBUTES

CLX_METHOD_ARRAY(
	clx::context::devices,
	::clGetContextInfo, CL_CONTEXT_DEVICES,
	device
)

#if CL_TARGET_VERSION >= 200
std::vector<clx::command_queue>
clx::context::command_queues_200(command_queue_flags flags) const {
	const auto& devices = this->devices();
	std::vector<command_queue> result;
	result.reserve(devices.size());
	for (const auto& device : devices) {
		result.emplace_back(device.queue_200(this->_ptr, flags));
	}
	return result;
}
#endif

#if CL_TARGET_VERSION <= 120 || defined(CL_USE_DEPRECATED_OPENCL_1_2_APIS)
std::vector<clx::command_queue>
clx::context::command_queues_100(command_queue_flags flags) const {
	const auto& devices = this->devices();
	std::vector<command_queue> result;
	result.reserve(devices.size());
	for (const auto& device : devices) {
		result.emplace_back(device.queue_100(this->_ptr, flags));
	}
	return result;
}
#endif

clx::program
clx::context::program(const std::string& src) const {
	const char* string = src.data();
	size_t length = src.length();
	int_type ret = 0;
	auto prg = ::clCreateProgramWithSource(
		this->_ptr,
		1,
		&string,
		&length,
		&ret
	);
	CLX_CHECK(ret);
	return static_cast<::clx::program>(prg);
}

clx::program
clx::context::program(const array_view<binary>& binaries) const {
	const auto& devices = this->devices();
	if (devices.size() != binaries.size()) {
		throw std::invalid_argument("different number of binaries and devices");
	}
	std::vector<size_t> sizes;
	sizes.reserve(binaries.size());
	std::vector<const unsigned char*> data;
	data.reserve(binaries.size());
	for (const auto& bin : binaries) {
		sizes.emplace_back(bin.size());
		data.emplace_back(bin.data());
	}
	int_type ret = 0;
	std::vector<errc> status(binaries.size());
	auto prg = ::clCreateProgramWithBinary(
		this->_ptr,
		devices.size(), downcast(devices.data()), sizes.data(),
		data.data(), downcast(status.data()),
		&ret
	);
	CLX_CHECK(ret);
	for (const auto& st : status) {
		if (st != errc::success) {
			CLX_THROW(st);
		}
	}
	return static_cast<::clx::program>(prg);
}

#if CL_TARGET_VERSION >= 200
clx::program
clx::context::program(const intermediate_language& il) const {
	int_type ret = 0;
	auto prg = ::clCreateProgramWithIL(this->_ptr, il.data(), il.size(), &ret);
	CLX_CHECK(ret);
	return static_cast<::clx::program>(prg);
}
#endif

#if CL_TARGET_VERSION >= 120
clx::program
clx::context::builtin_program(const std::string& names) const {
	const auto& devices = this->devices();
	int_type ret = 0;
	auto prg = ::clCreateProgramWithBuiltInKernels(
		this->_ptr, devices.size(), downcast(devices.data()),
		names.data(), &ret
	);
	CLX_CHECK(ret);
	return static_cast<::clx::program>(prg);
}
#endif

#if CL_TARGET_VERSION >= 120
clx::header
clx::context::header(const std::string& name, const std::string& src) const {
	return {this->program(src), name};
}
#endif

std::vector<clx::image_format>
clx::context::image_formats(memory_flags flags, memory_objects type) const {
	std::vector<image_format> result(4096 / sizeof(image_format));
	int_type ret = 0;
	bool success = false;
	unsigned_int_type actual_size = 0;
	while (!success) {
		ret = ::clGetSupportedImageFormats(
			this->_ptr, downcast(flags), downcast(type),
			result.size(), downcast(result.data()), &actual_size
		);
		result.resize(actual_size);
		if (errc(ret) != errc::invalid_value && result.size() <= actual_size) {
			CLX_CHECK(ret);
			success = true;
		}
	}
	return result;
}

CLX_METHOD_ARRAY(
	clx::context::properties,
	::clGetContextInfo, CL_CONTEXT_PROPERTIES,
	context_properties_type
);

CLX_METHOD_SCALAR(
	clx::context::num_references,
	::clGetContextInfo,
	unsigned_int_type,
	CL_CONTEXT_REFERENCE_COUNT
)

#if CL_TARGET_VERSION >= 120
CLX_METHOD_SCALAR(
	clx::context::num_devices,
	::clGetContextInfo,
	unsigned_int_type,
	CL_CONTEXT_NUM_DEVICES
)
#endif

#if CL_TARGET_VERSION >= 200
clx::image
clx::context::image_200(
	memory_flags flags,
	const image_format& format,
	const image_descriptor& descriptor,
	void* host_pointer
) const {
	int_type ret = 0;
	memory_type img = nullptr;
	img = ::clCreateImage(
		this->_ptr,
		static_cast<memory_flags_type>(flags),
		&format,
		&descriptor,
		host_pointer,
		&ret
	);
	CLX_CHECK(ret);
	return static_cast<::clx::image>(img);
}
#endif

#if CL_TARGET_VERSION <= 110 || defined(CL_USE_DEPRECATED_OPENCL_1_1_APIS)
clx::image
clx::context::image_100(
	memory_flags flags,
	const image_format& format,
	const image_descriptor& descriptor,
	void* host_pointer
) const {
	int_type ret = 0;
	memory_type img = nullptr;
	if (descriptor.depth() == 0) {
		img = ::clCreateImage2D(
			this->_ptr,
			static_cast<memory_flags_type>(flags),
			&format,
			descriptor.width(),
			descriptor.height(),
			descriptor.row_pitch(),
			host_pointer,
			&ret
		);
	} else {
		img = ::clCreateImage3D(
			this->_ptr,
			static_cast<memory_flags_type>(flags),
			&format,
			descriptor.width(),
			descriptor.height(),
			descriptor.depth(),
			descriptor.row_pitch(),
			descriptor.slice_pitch(),
			host_pointer,
			&ret
		);
	}
	CLX_CHECK(ret);
	return static_cast<::clx::image>(img);
}
#endif

clx::event
clx::context::event() const {
	int_type ret = 0;
	auto ev = ::clCreateUserEvent(this->_ptr, &ret);
	CLX_CHECK(ret);
	return static_cast<::clx::event>(ev);
}

clx::buffer
clx::context::buffer(
	memory_flags flags,
	size_t size,
	void* host_pointer
) const {
	int_type ret = 0;
	auto buf =
		::clCreateBuffer(
			this->_ptr,
			static_cast<memory_flags_type>(flags),
			size,
			host_pointer,
			&ret
		);
	CLX_CHECK(ret);
	return static_cast<::clx::buffer>(buf);
}

#if CL_TARGET_VERSION >= 200
clx::sampler
clx::context::sampler_200(const sampler_properties& prop) const {
	int_type ret = 0;
	auto props = prop(platform());
	auto sm = ::clCreateSamplerWithProperties(this->_ptr, props.data(), &ret);
	CLX_CHECK(ret);
	return static_cast<::clx::sampler>(sm);
}
#endif

#if CL_TARGET_VERSION <= 120 || defined(CL_USE_DEPRECATED_OPENCL_1_2_APIS)
clx::sampler
clx::context::sampler_100(const sampler_properties& prop) const {
	int_type ret = 0;
	auto sm = ::clCreateSampler(
		this->_ptr,
		static_cast<bool_type>(normalised),
		static_cast<addressing_mode_type>(amode),
		static_cast<filter_mode_type>(fmode),
		&ret
	);
	CLX_CHECK(ret);
	return static_cast<::clx::sampler>(sm);
}
#endif

#if CL_TARGET_VERSION >= 200
clx::pipe
clx::context::pipe(
	memory_flags flags,
	unsigned_int_type packet_size,
	unsigned_int_type max_packets
) const {
	int_type ret = 0;
	auto p = ::clCreatePipe(
		this->_ptr,
		static_cast<memory_flags_type>(flags),
		packet_size,
		max_packets,
		nullptr,
		&ret
	);
	CLX_CHECK(ret);
	return static_cast<::clx::pipe>(p);
}
#endif

#if CL_TARGET_VERSION >= 200
clx::svm_block
clx::context::shared_memory(
	svm_flags flags,
	size_t size,
	size_t alignment
) const {
	auto ptr = ::clSVMAlloc(this->_ptr, downcast(flags), size, alignment);
	if (!ptr) { throw std::bad_alloc{}; }
	return svm_block{ptr, size};
}
#endif

clx::platform
clx::context::platform() const {
	const auto& prop = this->properties();
	const auto nprops = prop.size();
	for (size_t i=0; i<nprops; ++i) {
		if (prop[i] == CL_CONTEXT_PLATFORM && i < nprops) {
			return ::clx::platform(reinterpret_cast<platform_type>(prop[i+1]));
		}
	}
	return clx::platform();
}

#if CL_TARGET_VERSION >= 120 && defined(cl_khr_terminate_context)
void
clx::context::terminate() {
	auto func = CLX_EXTENSION(clTerminateContextKHR, platform());
	CLX_CHECK(func(this->_ptr));
}
#endif

CLX_WARNING_POP
