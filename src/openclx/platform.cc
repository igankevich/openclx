#include <openclx/array_view>
#include <openclx/bits/macros>
#include <openclx/context>
#include <openclx/context_properties>
#include <openclx/device>
#include <openclx/downcast>
#include <openclx/error>
#include <openclx/extensions>
#include <openclx/platform>

namespace {

	const char* extension_function_names[] = {
		"clCreateCommandQueueWithPropertiesKHR",
		"clTerminateContextKHR",
		"clEnqueueMigrateMemObjectEXT",
		"clGetKernelSubGroupInfoKHR",
		"clGetGLContextInfoKHR",
		"clCreateEventFromEGLSyncKHR",
		"clImportMemoryARM",
		"clReleaseDeviceEXT",
		"clRetainDeviceEXT",
		"clCreateSubDevicesEXT",
		"clSVMAllocARM",
		"clSVMFreeARM",
		"clEnqueueSVMFreeARM",
		"clEnqueueSVMMemcpyARM",
		"clEnqueueSVMMemFillARM",
		"clEnqueueSVMMapARM",
		"clEnqueueSVMUnmapARM",
		"clSetKernelArgSVMPointerARM",
		"clSetKernelExecInfoARM",
		"clCreateAcceleratorINTEL",
		"clGetAcceleratorInfoINTEL",
		"clRetainAcceleratorINTEL",
		"clReleaseAcceleratorINTEL",
		"clGetDeviceImageInfoQCOM",
		"clGetDeviceIDsFromDX9MediaAdapterKHR",
		"clCreateFromDX9MediaSurfaceKHR",
		"clEnqueueAcquireDX9MediaSurfacesKHR",
		"clEnqueueReleaseDX9MediaSurfacesKHR",
		"clGetDeviceIDsFromD3D10KHR",
		"clCreateFromD3D10BufferKHR",
		"clCreateFromD3D10Texture2DKHR",
		"clCreateFromD3D10Texture3DKHR",
		"clEnqueueAcquireD3D10ObjectsKHR",
		"clEnqueueReleaseD3D10ObjectsKHR",
		"clGetDeviceIDsFromD3D11KHR",
		"clCreateFromD3D11BufferKHR",
		"clCreateFromD3D11Texture2DKHR",
		"clCreateFromD3D11Texture3DKHR",
		"clEnqueueAcquireD3D11ObjectsKHR",
		"clEnqueueReleaseD3D11ObjectsKHR",
		"clGetDeviceIDsFromDX9INTEL",
		"clCreateFromDX9MediaSurfaceINTEL",
		"clEnqueueAcquireDX9ObjectsINTEL",
		"clEnqueueReleaseDX9ObjectsINTEL",
		"clGetDeviceIDsFromVA_APIMediaAdapterINTEL",
		"clCreateFromVA_APIMediaSurfaceINTEL",
		"clEnqueueAcquireVA_APIMediaSurfacesINTEL",
		"clEnqueueReleaseVA_APIMediaSurfacesINTEL",
	};

}

CLX_METHOD_STRING(clx::platform::profile, ::clGetPlatformInfo, CL_PLATFORM_PROFILE);
CLX_METHOD_STRING(clx::platform::version, ::clGetPlatformInfo, CL_PLATFORM_VERSION);
CLX_METHOD_STRING(clx::platform::name, ::clGetPlatformInfo, CL_PLATFORM_NAME);
CLX_METHOD_STRING(clx::platform::vendor, ::clGetPlatformInfo, CL_PLATFORM_VENDOR);
CLX_METHOD_STRING2(
	clx::platform::extensions,
	::clGetPlatformInfo,
	::clx::extensions,
	CL_PLATFORM_EXTENSIONS
);
#if defined(CL_PLATFORM_ICD_SUFFIX_KHR)
CLX_METHOD_STRING(clx::platform::suffix, ::clGetPlatformInfo, CL_PLATFORM_ICD_SUFFIX_KHR);
#endif

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
	std::vector<platform_type> result(4096 / sizeof(platform));
	unsigned_int_type actual_size = 0;
	int_type ret;
	bool success = false;
	while (!success) {
		ret = ::clGetPlatformIDs(result.size(), result.data(), &actual_size);
		result.resize(actual_size);
		if (errc(ret) != errc::invalid_value && actual_size <= result.size()) {
			CLX_CHECK(ret);
			success = true;
		}
	}
	return std::vector<platform>(result.begin(), result.end());
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

#if CL_TARGET_VERSION >= 120 && defined(cl_khr_dx9_media_sharing)
std::vector<clx::device>
clx::platform::devices(
	d3d9::adapter_set flags,
	std::vector<std::pair<d3d9::adapter_type,void*>> adapters
) const {
	const auto nadapters = adapters;
	std::vector<d3d9::adapter_type> types;
	types.reserve(nadapters);
	std::vector<void*> ptrs;
	ptrs.reserve(nadapters);
	for (const auto& pair : adapters) {
		types.emplace_back(pair.first);
		ptrs.emplace_back(pair.second);
	}
	auto func = CLX_EXTENSION(clGetDeviceIDsFromDX9MediaAdapterKHR, *this);
	std::vector<device> result(4096 / sizeof(device));
	unsigned_int_type actual_size = 0;
	int_type ret;
	bool success = false;
	while (!success) {
		ret = func(
			this->_ptr, nadapters, types.data(), ptrs.data(), flags,
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
#endif

#if defined(cl_khr_d3d10_sharing)
std::vector<clx::device>
clx::platform::devices(d3d10::device_type* object, d3d10::device_set set) const {
	auto func = CLX_EXTENSION(clGetDeviceIDsFromD3D10KHR, *this);
	std::vector<device> result(4096 / sizeof(device));
	unsigned_int_type actual_size = 0;
	int_type ret;
	bool success = false;
	while (!success) {
		ret = func(
			this->_ptr, CL_D3D10_DEVICE_KHR, object, set,
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
clx::platform::devices(d3d10::dxgi_adapter_type* object, d3d10::device_set set) const {
	auto func = CLX_EXTENSION(clGetDeviceIDsFromD3D10KHR, *this);
	std::vector<device> result(4096 / sizeof(device));
	unsigned_int_type actual_size = 0;
	int_type ret;
	bool success = false;
	while (!success) {
		ret = func(
			this->_ptr, CL_D3D10_DXGI_ADAPTER_KHR, object, set,
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
#endif

#if CL_TARGET_VERSION >= 120 && defined(cl_khr_d3d11_sharing)
std::vector<clx::device>
clx::platform::devices(d3d11::device_type* object, d3d11::device_set set) const {
	auto func = CLX_EXTENSION(clGetDeviceIDsFromD3D11KHR, *this);
	std::vector<device> result(4096 / sizeof(device));
	unsigned_int_type actual_size = 0;
	int_type ret;
	bool success = false;
	while (!success) {
		ret = func(
			this->_ptr, CL_D3D11_DEVICE_KHR, object, set,
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
clx::platform::devices(d3d11::dxgi_adapter_type* object, d3d11::device_set set) const {
	auto func = CLX_EXTENSION(clGetDeviceIDsFromD3D11KHR, *this);
	std::vector<device> result(4096 / sizeof(device));
	unsigned_int_type actual_size = 0;
	int_type ret;
	bool success = false;
	while (!success) {
		ret = func(
			this->_ptr, CL_D3D11_DXGI_ADAPTER_KHR, object, set,
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
#endif

#if defined(cl_intel_dx9_media_sharing)
std::vector<clx::device>
clx::platform::devices(intel::device_source src, void* object, intel::device_set set) const {
	auto func = CLX_EXTENSION(clGetDeviceIDsFromDX9INTEL, *this);
	std::vector<device> result(4096 / sizeof(device));
	unsigned_int_type actual_size = 0;
	int_type ret;
	bool success = false;
	while (!success) {
		ret = func(
			this->_ptr, src, object, set,
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
#endif

#if defined(cl_intel_va_api_media_sharing)
std::vector<clx::device>
clx::platform::devices(
	intel::va::device_source src,
	void* object,
	intel::va::device_set set
) const {
	auto func = CLX_EXTENSION(clGetDeviceIDsFromVA_APIMediaAdapterINTEL, *this);
	std::vector<device> result(4096 / sizeof(device));
	unsigned_int_type actual_size = 0;
	int_type ret;
	bool success = false;
	while (!success) {
		ret = func(
			this->_ptr, downcast(src), object, downcast(set),
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
#endif

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

clx::context
clx::platform::context(
	const context_properties& properties,
	const array_view<device>& devices
) const {
	auto prop = properties(extensions());
	prop << CL_CONTEXT_PLATFORM << this->_ptr << 0;
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

clx::context clx::platform::context(
	const context_properties& properties,
	device_flags types
) const {
	auto prop = properties(extensions());
	prop << CL_CONTEXT_PLATFORM << this->_ptr << 0;
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

clx::platform::function_address
clx::platform::do_function(extension_function func) const {
	if (this->_functions.empty()) {
		this->_functions.resize(static_cast<size_t>(extension_function::size));
	}
	auto i = static_cast<size_t>(func);
	auto& ptr = this->_functions[i];
	if (!ptr) { ptr = this->extension_function_address(extension_function_names[i]); }
	if (!ptr) { throw std::runtime_error(extension_function_names[i]); }
	return ptr;
}

