#include <openclx/context>
#include <openclx/memory_object>
#include <openclx/bits/macros>

CLX_METHOD_SCALAR(
	clx::memory_object::type,
	::clGetMemObjectInfo,
	memory_objects,
	CL_MEM_TYPE
)

CLX_METHOD_SCALAR(
	clx::memory_object::flags,
	::clGetMemObjectInfo,
	memory_flags,
	CL_MEM_FLAGS
)

CLX_METHOD_SCALAR(
	clx::memory_object::size,
	::clGetMemObjectInfo,
	size_t,
	CL_MEM_SIZE
)

CLX_METHOD_SCALAR(
	clx::memory_object::host_pointer,
	::clGetMemObjectInfo,
	void*,
	CL_MEM_HOST_PTR
)

CLX_METHOD_SCALAR(
	clx::memory_object::num_maps,
	::clGetMemObjectInfo,
	unsigned_int_type,
	CL_MEM_MAP_COUNT
)

CLX_METHOD_SCALAR(
	clx::memory_object::num_references,
	::clGetMemObjectInfo,
	unsigned_int_type,
	CL_MEM_REFERENCE_COUNT
)

CLX_METHOD_SCALAR(
	clx::memory_object::context,
	::clGetMemObjectInfo,
	::clx::context,
	CL_MEM_CONTEXT
)
