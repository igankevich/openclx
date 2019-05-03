#include <openclx/bits/macros>
#include <openclx/d3d9/media_surface>

#if defined(cl_khr_dx9_media_sharing)
CLX_METHOD_SCALAR(
	clx::d3d9::media_surface::adapter,
	::clGetMemObjectInfo,
	adapter_type,
	CL_MEM_DX9_MEDIA_ADAPTER_TYPE_KHR
);

CLX_METHOD_SCALAR(
	clx::d3d9::media_surface::surface,
	::clGetMemObjectInfo,
	surface_info,
	CL_MEM_DX9_MEDIA_SURFACE_INFO_KHR
);

CLX_METHOD_SCALAR(
	clx::d3d9::media_surface::plane,
	::clGetImageInfo,
	unsigned_int_type,
	CL_IMAGE_DX9_MEDIA_PLANE_KHR
);
#endif

