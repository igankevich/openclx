#include <openclx/bits/macros>
#include <openclx/context>
#include <openclx/device>
#include <openclx/error>
#include <openclx/platform>

#define CLX_PLATFORM_GET_STRING(name, key) \
	std::string \
	clx::platform::name() const { \
		CLX_GET_STRING(::clGetPlatformInfo, key); \
	}

#define CLX_PLATFORM_GET_SCALAR4(name, key, type, return_type) \
	auto \
	clx::platform::name() const -> return_type { \
		CLX_GET_SCALAR4(::clGetPlatformInfo, key, type, return_type) \
	}

CLX_PLATFORM_GET_STRING(profile, CL_PLATFORM_PROFILE);
CLX_PLATFORM_GET_STRING(version, CL_PLATFORM_VERSION);
CLX_PLATFORM_GET_STRING(name, CL_PLATFORM_NAME);
CLX_PLATFORM_GET_STRING(vendor, CL_PLATFORM_VENDOR);
CLX_PLATFORM_GET_STRING(extensions, CL_PLATFORM_EXTENSIONS);
CLX_PLATFORM_GET_SCALAR4(timer_resolution, CL_PLATFORM_HOST_TIMER_RESOLUTION, unsigned_long_type, nanoseconds);

std::vector<clx::platform>
clx::platforms() {
	std::vector<platform> result(4096 / sizeof(platform));
	unsigned_int_type actual_size = 0;
	int_type ret;
	bool success = false;
	while (!success) {
		ret = ::clGetPlatformIDs(
			result.size(),
			reinterpret_cast<platform_type*>(result.data()),
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

std::vector<clx::device>
clx::platform::devices(device_flags types) const {
	std::vector<device> result(4096 / sizeof(device));
	unsigned_int_type actual_size = 0;
	int_type ret;
	bool success = false;
	while (!success) {
		ret = ::clGetDeviceIDs(
			this->_ptr,
			static_cast<device_flags_type>(types),
			result.size(),
			reinterpret_cast<device_type*>(result.data()),
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

clx::context clx::platform::context(const std::vector<device>& devices) const {
	std::vector<context_properties_type> prop{
		context_properties_type(CL_CONTEXT_PLATFORM),
		context_properties_type(this->_ptr),
		context_properties_type(0)
	};
	int_type ret = 0;
	auto ctx = ::clCreateContext(
		prop.data(),
		devices.size(),
		reinterpret_cast<const device_type*>(devices.data()),
		nullptr,
		nullptr,
		&ret
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
