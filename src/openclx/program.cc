#include <memory>

#include <openclx/array_view>
#include <openclx/binary>
#include <openclx/bits/macros>
#include <openclx/build_status>
#include <openclx/context>
#include <openclx/device>
#include <openclx/downcast>
#include <openclx/kernel>
#include <openclx/program>

void
clx::program::build(const std::string& options, const device_array& devices) {
	CLX_CHECK(::clBuildProgram(
		this->_ptr, devices.size(), downcast(devices.data()),
		options.data(), nullptr, nullptr
	));
}

clx::build_status
clx::program::build_status(const device& dev) const {
	CLX_BODY_SCALAR(
		::clGetProgramBuildInfo, ::clx::build_status,
		dev.get(), CL_PROGRAM_BUILD_STATUS
	)
}

#if CL_TARGET_VERSION >= 120
clx::binary_types
clx::program::binary_type(const device& dev) const {
	CLX_BODY_SCALAR(
		::clGetProgramBuildInfo, ::clx::binary_types,
		dev.get(), CL_PROGRAM_BINARY_TYPE
	)
}
#endif

#if CL_TARGET_VERSION >= 200
size_t
clx::program::size_of_global_variables(const device& dev) const {
	CLX_BODY_SCALAR(
		::clGetProgramBuildInfo, size_t,
		dev.get(), CL_PROGRAM_BUILD_GLOBAL_VARIABLE_TOTAL_SIZE
	)
}
#endif

std::string
clx::program::options(const device& dev) const {
	std::string value(4096, ' ');
	int_type ret;
	bool success = false;
	size_t actual_size = 0;
	while (!success) {
		ret = ::clGetProgramBuildInfo(
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
		ret = ::clGetProgramBuildInfo(
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
	static_assert(
		sizeof(std::unique_ptr<unsigned char>) == sizeof(unsigned char*),
		"bad size"
	);
	std::vector<std::unique_ptr<unsigned char[]>> binaries;
	binaries.reserve(ndevices);
	for (const auto& s : sizes) {
		binaries.emplace_back(new unsigned char[s]);
	}
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
		result.emplace_back(binaries[i].get(), sizes[i]);
	}
	return result;
}

#if CL_TARGET_VERSION >= 120
void
clx::program::compile(
	const std::string& options,
	const header_array& headers,
	const device_array& devices
) {
	std::vector<program_type> programs;
	programs.reserve(headers.size());
	std::vector<const char*> names;
	names.reserve(headers.size());
	for (const auto& h : headers) {
		programs.emplace_back(h.program.get());
		names.emplace_back(h.name.data());
	}
	CLX_CHECK(::clCompileProgram(
		this->_ptr,
		devices.size(), downcast(devices.data()),
		options.data(),
		headers.size(), programs.data(), names.data(),
		nullptr, nullptr
	));
}
#endif

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
			downcast(result.data()),
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

CLX_METHOD_SCALAR(clx::program::context, ::clGetProgramInfo, ::clx::context, CL_PROGRAM_CONTEXT)
CLX_METHOD_SCALAR(clx::program::num_references, ::clGetProgramInfo, unsigned_int_type, CL_PROGRAM_REFERENCE_COUNT)
CLX_METHOD_SCALAR(clx::program::num_devices, ::clGetProgramInfo, unsigned_int_type, CL_PROGRAM_NUM_DEVICES)
CLX_METHOD_ARRAY(clx::program::devices, ::clGetProgramInfo, CL_PROGRAM_DEVICES, device)
CLX_METHOD_STRING(clx::program::source, ::clGetProgramInfo, CL_PROGRAM_SOURCE)

#if CL_TARGET_VERSION >= 210
clx::intermediate_language
clx::program::intermediate_language() const {
	size_t actual_size = 0;
	CLX_CHECK(::clGetProgramInfo(
		this->_ptr, CL_PROGRAM_IL,
		0, nullptr, &actual_size
	));
	::clx::intermediate_language il(actual_size);
	CLX_CHECK(::clGetProgramInfo(
		this->_ptr, CL_PROGRAM_IL,
		actual_size, il.data(), nullptr
	));
	return il;
}
#endif

#if CL_TARGET_VERSION >= 120
clx::program
clx::link(
	const array_view<program>& programs,
	const std::string& options,
	const array_view<device>& devices
) {
	if (programs.empty()) {
		throw std::invalid_argument("empty programs");
	}
	int_type ret = 0;
	auto prog =
		::clLinkProgram(
			programs.front().context().get(),
			devices.size(), downcast(devices.data()),
			options.data(),
			programs.size(), downcast(programs.data()),
			nullptr, nullptr, &ret
		);
	CLX_CHECK(ret);
	return static_cast<program>(prog);
}
#endif

