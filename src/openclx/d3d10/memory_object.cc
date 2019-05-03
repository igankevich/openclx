#include <openclx/bits/macros>
#include <openclx/d3d10/memory_object>

#if defined(cl_khr_d3d10_sharing)
CLX_METHOD_SCALAR(
	clx::d3d10::memory_object::resource,
	::clGetMemObjectInfo,
	resource_type,
	CL_MEM_D3D10_RESOURCE_KHR
);
CLX_METHOD_SCALAR(
	clx::d3d10::memory_object::subresource,
	::clGetImageInfo,
	resource_type,
	CL_MEM_D3D10_SUBRESOURCE_KHR
);
#endif
