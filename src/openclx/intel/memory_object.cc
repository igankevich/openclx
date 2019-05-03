#include <openclx/bits/macros>
#include <openclx/intel/memory_object>

#if defined(cl_intel_dx9_media_sharing)
CLX_METHOD_SCALAR(
	clx::intel::memory_object::resource,
	::clGetMemObjectInfo,
	surface_type,
	CL_MEM_DX9_RESOURCE_INTEL
);
CLX_METHOD_SCALAR(
	clx::intel::memory_object::shared_handle,
	::clGetMemObjectInfo,
	handle_type,
	CL_MEM_DX9_SHARED_HANDLE_INTEL
);
CLX_METHOD_SCALAR(
	clx::intel::memory_object::plane,
	::clGetImageInfo,
	uint_type,
	CL_IMAGE_DX9_PLANE_INTEL
);
#endif

