#include <openclx/bits/macros>
#include <openclx/context>
#include <openclx/device>
#include <openclx/kernel>
#include <openclx/program>

clx::kernel
clx::kernel::copy() const {
	int_type ret = 0;
	auto krnl = ::clCloneKernel(this->_ptr, &ret);
	CLX_CHECK(ret);
	return static_cast<kernel>(krnl);
}

CLX_GET_STRING3(clx::kernel::name, ::clGetKernelInfo, CL_KERNEL_FUNCTION_NAME)
CLX_GET_SCALAR5(
	clx::kernel::num_arguments,
	::clGetKernelInfo,
	CL_KERNEL_NUM_ARGS,
	unsigned_int_type,
	unsigned_int_type
)
CLX_GET_SCALAR5(
	clx::kernel::num_references,
	::clGetKernelInfo,
	CL_KERNEL_REFERENCE_COUNT,
	unsigned_int_type,
	unsigned_int_type
)
CLX_GET_SCALAR5(
	clx::kernel::context,
	::clGetKernelInfo,
	CL_KERNEL_CONTEXT,
	context_type,
	::clx::context
)
CLX_GET_SCALAR5(
	clx::kernel::program,
	::clGetKernelInfo,
	CL_KERNEL_PROGRAM,
	program_type,
	::clx::program
)

clx::work_group
clx::kernel::work_group(const device& dev) const {
	#define OPENCLX_WG_FIELD(field, key) \
		CLX_CHECK(::clGetKernelWorkGroupInfo( \
			this->_ptr, \
			dev.get(), \
			key, \
			sizeof(field), \
			&field, \
			nullptr \
		))
	::clx::work_group wg{};
	OPENCLX_WG_FIELD(wg.size, CL_KERNEL_WORK_GROUP_SIZE);
	OPENCLX_WG_FIELD(wg.size_hint, CL_KERNEL_COMPILE_WORK_GROUP_SIZE);
	OPENCLX_WG_FIELD(wg.local_memory_size, CL_KERNEL_LOCAL_MEM_SIZE);
	#if CLX_OPENCL_VERSION >= 110
	OPENCLX_WG_FIELD(wg.private_memory_size, CL_KERNEL_PRIVATE_MEM_SIZE);
	OPENCLX_WG_FIELD(wg.size_multiple, CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE);
	#endif
	return wg;
	#undef OPENCLX_WG_FIELD
}
