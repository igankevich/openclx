#include <openclx/bits/macros>
#include <openclx/context>
#include <openclx/device>
#include <openclx/kernel>
#include <openclx/program>

#define CLX_PROGRAM_GET_STRING(name, key) \
	std::string \
	clx::program::name() const { \
		CLX_GET_STRING(::clGetProgramInfo, key); \
	}

#define CLX_PROGRAM_GET_SCALAR4(name, key, type, return_type) \
	auto \
	clx::program::name() const -> return_type { \
		CLX_GET_SCALAR4(::clGetProgramInfo, key, type, return_type) \
	}

#define CLX_PROGRAM_GET_ARRAY(name, key, type, return_type) \
	auto \
	clx::program::name() const -> std::vector<return_type> { \
		CLX_GET_ARRAY(::clGetProgramInfo, key, type, return_type) \
	}

void
clx::program::build(const std::string& options) {
	const auto& devices = this->devices();
	CLX_CHECK(::clBuildProgram(
		this->_ptr,
		devices.size(),
		reinterpret_cast<const device_type*>(devices.data()),
		options.data(),
		nullptr,
		nullptr
	));
}

clx::build_status
clx::program::build_status(const device& dev) const {
	::clx::build_status value = ::clx::build_status::none;
	CLX_CHECK(::clGetProgramBuildInfo(
		this->_ptr,
		dev.get(),
		CL_PROGRAM_BUILD_STATUS,
		sizeof(value),
		&value,
		nullptr
	));
	return value;
}

std::string
clx::program::options(const device& dev) const {
	std::string value(4096, ' ');
	int_type ret;
	bool success = false;
	size_t actual_size = 0;
	while (!success) {
		ret = clGetProgramBuildInfo(
			this->_ptr,
			dev.get(),
			CL_PROGRAM_BUILD_OPTIONS,
			sizeof(value),
			&value,
			nullptr
		);
		value.resize(actual_size);
		if (errc(ret) != errc::invalid_value && actual_size <= value.size()) {
			CLX_CHECK(ret);
			success = true;
		}
	}
	return value;
}

std::string
clx::program::build_log(const device& dev) const {
	std::string value(4096, ' ');
	int_type ret;
	bool success = false;
	size_t actual_size = 0;
	while (!success) {
		ret = clGetProgramBuildInfo(
			this->_ptr,
			dev.get(),
			CL_PROGRAM_BUILD_LOG,
			sizeof(value),
			&value,
			nullptr
		);
		value.resize(actual_size);
		if (errc(ret) != errc::invalid_value && actual_size <= value.size()) {
			CLX_CHECK(ret);
			success = true;
		}
	}
	return value;
}

std::vector<clx::binary>
clx::program::binaries() const {
	unsigned_int_type ndevices = this->num_devices();
	std::vector<size_t> sizes(ndevices);
	CLX_CHECK(::clGetProgramInfo(
		this->_ptr,
		CL_PROGRAM_BINARY_SIZES,
		sizes.size()*sizeof(size_t),
		sizes.data(),
		nullptr
	));
	std::vector<unsigned char*> binaries(ndevices);
	CLX_CHECK(::clGetProgramInfo(
		this->_ptr,
		CL_PROGRAM_BINARIES,
		binaries.size()*sizeof(unsigned char*),
		binaries.data(),
		nullptr
	));
	std::vector<binary> result;
	result.reserve(ndevices);
	for (unsigned_int_type i=0; i<ndevices; ++i) {
		result.emplace_back(binaries[i], sizes[i]);
	}
	return result;
}

std::vector<clx::kernel>
clx::program::kernels() const {
	std::vector<::clx::kernel> result(4096 / sizeof(::clx::kernel));
	int_type ret = 0;
	bool success = false;
	unsigned_int_type actual_size = 0;
	while (!success) {
		ret = ::clCreateKernelsInProgram(
			this->_ptr,
			result.size(),
			reinterpret_cast<kernel_type*>(result.data()),
			&actual_size
		);
		result.resize(actual_size);
		if (errc(ret) != errc::invalid_value && actual_size <= result.size()) {
			CLX_CHECK(ret);
			success = true;
		}
	}
	return result;
}

clx::kernel
clx::program::kernel(const char* name) const {
	int_type ret = 0;
	auto krnl = ::clCreateKernel(this->_ptr, name, &ret);
	CLX_CHECK(ret);
	return static_cast<::clx::kernel>(krnl);
}

CLX_PROGRAM_GET_SCALAR4(context, CL_PROGRAM_CONTEXT, context_type, ::clx::context)
CLX_PROGRAM_GET_SCALAR4(num_references, CL_PROGRAM_REFERENCE_COUNT, unsigned_int_type, unsigned_int_type)
CLX_PROGRAM_GET_SCALAR4(num_devices, CL_PROGRAM_NUM_DEVICES, unsigned_int_type, unsigned_int_type)
CLX_PROGRAM_GET_ARRAY(devices, CL_PROGRAM_DEVICES, device_type, device)
CLX_PROGRAM_GET_STRING(source, CL_PROGRAM_SOURCE)
