#include <openclx/addressing_mode>
#include <openclx/binary>
#include <openclx/bits/macros>
#include <openclx/bits/pragmas>
#include <openclx/buffer>
#include <openclx/command_queue>
#include <openclx/context>
#include <openclx/device>
#include <openclx/event>
#include <openclx/image>
#include <openclx/pipe>
#include <openclx/program>
#include <openclx/sampler>

CLX_WARNING_PUSH
CLX_IGNORED_ATTRIBUTES

CLX_METHOD_ARRAY(
	clx::context::devices,
	::clGetContextInfo, CL_CONTEXT_DEVICES,
	device
)

std::vector<clx::command_queue>
clx::context::command_queues(command_queue_flags flags) const {
	const auto& devices = this->devices();
	std::vector<command_queue> result;
	result.reserve(devices.size());
	for (const auto& device : devices) {
		result.emplace_back(device.queue(this->_ptr, flags));
	}
	return result;
}

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
clx::context::program(const std::vector<binary>& binaries) const {
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
		devices.size(),
		reinterpret_cast<const device_type*>(devices.data()),
		sizes.data(),
		data.data(),
		reinterpret_cast<int_type*>(status.data()),
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

#if CL_TARGET_VERSION >= 120
clx::program
clx::context::builtin_program(const std::string& names) const {
	const auto& devices = this->devices();
	int_type ret = 0;
	auto prg = ::clCreateProgramWithBuiltInKernels(
		this->_ptr,
		devices.size(),
		reinterpret_cast<const device_type*>(devices.data()),
		names.data(),
		&ret
	);
	CLX_CHECK(ret);
	return static_cast<::clx::program>(prg);
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
			this->_ptr,
			static_cast<memory_flags_type>(flags),
			static_cast<memory_objects_type>(type),
			result.size(),
			reinterpret_cast<image_format_type*>(result.data()),
			&actual_size
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
	::clGetContextInfo, CL_CONTEXT_REFERENCE_COUNT,
	unsigned_int_type
)

#if CL_TARGET_VERSION >= 120
CLX_METHOD_SCALAR(
	clx::context::num_devices,
	::clGetContextInfo, CL_CONTEXT_NUM_DEVICES,
	unsigned_int_type
)
#endif

clx::image
clx::context::image(
	memory_flags flags,
	const image_format& format,
	const image_descriptor& descriptor,
	void* host_pointer
) const {
	int_type ret = 0;
	memory_type img = nullptr;
	#if CL_TARGET_VERSION >= 120
	img =
		::clCreateImage(
			this->_ptr,
			static_cast<memory_flags_type>(flags),
			&format,
			&descriptor,
			host_pointer,
			&ret
		);
	#else
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
	#endif
	CLX_CHECK(ret);
	return static_cast<::clx::image>(img);
}

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

clx::sampler
clx::context::sampler(
	bool normalised,
	addressing_mode amode,
	filter_mode fmode
) const {
	int_type ret = 0;
	#if CL_TARGET_VERSION >= 200
	std::vector<sampler_properties_type> props{
		sampler_properties_type(CL_SAMPLER_NORMALIZED_COORDS),
		sampler_properties_type(normalised),
		sampler_properties_type(CL_SAMPLER_ADDRESSING_MODE),
		sampler_properties_type(amode),
		sampler_properties_type(CL_SAMPLER_FILTER_MODE),
		sampler_properties_type(fmode),
		sampler_properties_type(0)
	};
	auto sm = ::clCreateSamplerWithProperties(this->_ptr, props.data(), &ret);
	#else
	auto sm =
		::clCreateSampler(
			this->_ptr,
			static_cast<bool_type>(normalised),
			static_cast<addressing_mode_type>(amode),
			static_cast<filter_mode_type>(fmode),
			&ret
		);
	#endif
	CLX_CHECK(ret);
	return static_cast<::clx::sampler>(sm);
}

#if CL_TARGET_VERSION >= 200
::clx::pipe
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

CLX_WARNING_POP
