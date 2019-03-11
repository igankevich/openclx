#include <openclx/bits/macros>
#include <openclx/command_queue>
#include <openclx/context>
#include <openclx/device>
#include <openclx/program>

std::vector<clx::device>
clx::context::devices() const {
	CLX_GET_ARRAY(::clGetContextInfo, CL_CONTEXT_DEVICES, device_type, device);
}

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

std::vector<clx::context_properties_type>
clx::context::properties() const {
	CLX_GET_ARRAY(
		::clGetContextInfo,
		CL_CONTEXT_PROPERTIES,
		context_properties_type,
		context_properties_type
	);
}

clx::unsigned_int_type
clx::context::num_references() const {
	CLX_GET_SCALAR3(::clGetContextInfo, CL_CONTEXT_REFERENCE_COUNT, unsigned_int_type)
}

#if CL_TARGET_VERSION >= 120
clx::unsigned_int_type
clx::context::num_devices() const {
	CLX_GET_SCALAR3(::clGetContextInfo, CL_CONTEXT_NUM_DEVICES, unsigned_int_type)
}
#endif

#if CL_TARGET_VERSION >= 120
clx::image
clx::context::image(
	memory_flags flags,
	const image_format& format,
	const image_descriptor& descriptor,
	void* host_pointer
) const {
	int_type ret = 0;
	auto img =
		::clCreateImage(
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

clx::event
clx::context::event() const {
	int_type ret = 0;
	auto ev = ::clCreateUserEvent(this->_ptr, &ret);
	CLX_CHECK(ret);
	return static_cast<::clx::event>(ev);
}

