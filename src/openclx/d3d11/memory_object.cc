#include <openclx/bits/macros>
#include <openclx/d3d11/memory_object>

#if CL_TARGET_VERSION >= 120 && defined(cl_khr_d3d11_sharing)
CLX_METHOD_SCALAR(
	clx::d3d11::memory_object::resource,
	::clGetMemObjectInfo,
	resource_type,
	CL_MEM_D3D11_RESOURCE_KHR
);
CLX_METHOD_SCALAR(
	clx::d3d11::memory_object::subresource,
	::clGetImageInfo,
	resource_type,
	CL_MEM_D3D11_SUBRESOURCE_KHR
);
#endif

