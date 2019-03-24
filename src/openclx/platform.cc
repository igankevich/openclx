#include <openclx/array_view>
#include <openclx/bits/macros>
#include <openclx/context>
#include <openclx/device>
#include <openclx/downcast>
#include <openclx/error>
#include <openclx/platform>

CLX_METHOD_STRING(clx::platform::profile, ::clGetPlatformInfo, CL_PLATFORM_PROFILE);
CLX_METHOD_STRING(clx::platform::version, ::clGetPlatformInfo, CL_PLATFORM_VERSION);
CLX_METHOD_STRING(clx::platform::name, ::clGetPlatformInfo, CL_PLATFORM_NAME);
CLX_METHOD_STRING(clx::platform::vendor, ::clGetPlatformInfo, CL_PLATFORM_VENDOR);
CLX_METHOD_STRING(clx::platform::extensions, ::clGetPlatformInfo, CL_PLATFORM_EXTENSIONS);

#if CL_TARGET_VERSION >= 210
CLX_METHOD_SCALAR(
	clx::platform::timer_resolution,
	::clGetPlatformInfo,
	nanoseconds,
	CL_PLATFORM_HOST_TIMER_RESOLUTION
);
#endif

std::vector<clx::platform>
clx::platforms() {
	std::vector<platform> result(4096 / sizeof(platform));
	unsigned_int_type actual_size = 0;
	int_type ret;
	bool success = false;
	while (!success) {
		ret = ::clGetPlatformIDs(
			result.size(), downcast(result.data()), &actual_size
		);
		result.resize(actual_size);
		if (errc(ret) != errc::invalid_value && actual_size <= result.size()) {
			CLX_CHECK(ret);
			success = true;
		}
	}
	return result;
}

std::vector<clx::device>
clx::platform::devices(device_flags types) const {
	std::vector<device> result(4096 / sizeof(device));
	unsigned_int_type actual_size = 0;
	int_type ret;
	bool success = false;
	while (!success) {
		ret = ::clGetDeviceIDs(
			this->_ptr, static_cast<device_flags_type>(types),
			result.size(), downcast(result.data()),
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

clx::context clx::platform::context(const array_view<device>& devices) const {
	std::vector<context_properties_type> prop{
		context_properties_type(CL_CONTEXT_PLATFORM),
		context_properties_type(this->_ptr),
		context_properties_type(0)
	};
	int_type ret = 0;
	auto ctx = ::clCreateContext(
		prop.data(), devices.size(), downcast(devices.data()),
		nullptr, nullptr, &ret
	);
	CLX_CHECK(ret);
	return static_cast<clx::context>(ctx);
}

clx::context clx::platform::context(device_flags types) const {
	std::vector<context_properties_type> prop{
		context_properties_type(CL_CONTEXT_PLATFORM),
		context_properties_type(this->_ptr),
		context_properties_type(0)
	};
	int_type ret = 0;
	auto ctx =
		::clCreateContextFromType(
			prop.data(),
			static_cast<device_flags_type>(types),
			nullptr,
			nullptr,
			&ret
		);
	CLX_CHECK(ret);
	return static_cast<clx::context>(ctx);
}
