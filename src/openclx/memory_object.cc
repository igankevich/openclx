#include <openclx/context>
#include <openclx/memory_object>
#include <openclx/bits/macros>

CLX_METHOD_SCALAR(
	clx::memory_object::type,
	::clGetMemObjectInfo,
	CL_MEM_TYPE,
	memory_objects
)
CLX_METHOD_SCALAR(
	clx::memory_object::flags,
	::clGetMemObjectInfo,
	CL_MEM_FLAGS,
	memory_flags
)
CLX_METHOD_SCALAR(
	clx::memory_object::size,
	::clGetMemObjectInfo,
	CL_MEM_SIZE,
	size_t
)
CLX_METHOD_SCALAR(
	clx::memory_object::host_pointer,
	::clGetMemObjectInfo,
	CL_MEM_HOST_PTR,
	void*
)
CLX_METHOD_SCALAR(
	clx::memory_object::num_maps,
	::clGetMemObjectInfo,
	CL_MEM_MAP_COUNT,
	unsigned_int_type
)
CLX_METHOD_SCALAR(
	clx::memory_object::num_references,
	::clGetMemObjectInfo,
	CL_MEM_REFERENCE_COUNT,
	unsigned_int_type
)
CLX_METHOD_SCALAR(
	clx::memory_object::context,
	::clGetMemObjectInfo,
	CL_MEM_CONTEXT,
	::clx::context
)
