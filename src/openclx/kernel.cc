#include <openclx/bits/macros>
#include <openclx/context>
#include <openclx/device>
#include <openclx/kernel>
#include <openclx/kernel_argument>
#include <openclx/program>

clx::kernel
clx::kernel::copy() const {
	int_type ret = 0;
	auto krnl = ::clCloneKernel(this->_ptr, &ret);
	CLX_CHECK(ret);
	return static_cast<kernel>(krnl);
}

#if CL_TARGET_VERSION >= 120
clx::kernel_argument
clx::kernel::argument(unsigned_int_type i) const {
	return kernel_argument{*this,i};
}
#endif

CLX_METHOD_STRING(clx::kernel::name, ::clGetKernelInfo, CL_KERNEL_FUNCTION_NAME)

CLX_METHOD_SCALAR(
	clx::kernel::num_arguments,
	::clGetKernelInfo,
	unsigned_int_type,
	CL_KERNEL_NUM_ARGS
)

CLX_METHOD_SCALAR(
	clx::kernel::num_references,
	::clGetKernelInfo,
	unsigned_int_type,
	CL_KERNEL_REFERENCE_COUNT
)

CLX_METHOD_SCALAR(
	clx::kernel::context,
	::clGetKernelInfo,
	::clx::context,
	CL_KERNEL_CONTEXT
)

CLX_METHOD_SCALAR(
	clx::kernel::program,
	::clGetKernelInfo,
	::clx::program,
	CL_KERNEL_PROGRAM
)

clx::work_group
clx::kernel::work_group(const device& dev) const {
	#define CLX_WG_FIELD(field, key) \
		CLX_CHECK(::clGetKernelWorkGroupInfo( \
			this->_ptr, \
			dev.get(), \
			key, \
			sizeof(field), \
			&field, \
			nullptr \
		))
	::clx::work_group wg{};
	CLX_WG_FIELD(wg.size, CL_KERNEL_WORK_GROUP_SIZE);
	CLX_WG_FIELD(wg.size_hint, CL_KERNEL_COMPILE_WORK_GROUP_SIZE);
	CLX_WG_FIELD(wg.local_memory_size, CL_KERNEL_LOCAL_MEM_SIZE);
	#if CL_TARGET_VERSION >= 110
	CLX_WG_FIELD(wg.private_memory_size, CL_KERNEL_PRIVATE_MEM_SIZE);
	CLX_WG_FIELD(wg.size_multiple, CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE);
	#endif
	return wg;
	#undef CLX_WG_FIELD
}
