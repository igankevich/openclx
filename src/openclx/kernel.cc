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

#if CL_TARGET_VERSION >= 200
void
clx::kernel::svm_pointers_200(const array_view<void*>& pointers) {
	CLX_CHECK(::clSetKernelExecInfo(
		this->_ptr, CL_KERNEL_EXEC_INFO_SVM_PTRS,
		pointers.size()*sizeof(void*), pointers.data()
	));
}

void
clx::kernel::fine_grain_system_svm_pointers_200(bool b) {
	bool_type value = static_cast<bool_type>(b);
	CLX_CHECK(::clSetKernelExecInfo(
		this->_ptr, CL_KERNEL_EXEC_INFO_SVM_FINE_GRAIN_SYSTEM,
		sizeof(bool_type), &value
	));
}
#endif

#if CL_TARGET_VERSION >= 120 && defined(cl_arm_shared_virtual_memory)
void
clx::kernel::svm_pointers_arm(const array_view<void*>& pointers) {
	auto func = CLX_EXTENSION(clSetKernelExecInfoARM, context().platform());
	CLX_CHECK(func(
		this->_ptr, CL_KERNEL_EXEC_INFO_SVM_PTRS_ARM,
		pointers.size()*sizeof(void*), pointers.data()
	));
}

void
clx::kernel::fine_grain_system_svm_pointers_arm(bool b) {
	auto func = CLX_EXTENSION(clSetKernelExecInfoARM, context().platform());
	bool_type value = static_cast<bool_type>(b);
	CLX_CHECK(func(
		this->_ptr, CL_KERNEL_EXEC_INFO_SVM_FINE_GRAIN_SYSTEM_ARM,
		sizeof(bool_type), &value
	));
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
	CLX_WG_FIELD(wg.size_multiple, CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE);
	#if CL_TARGET_VERSION >= 110
	CLX_WG_FIELD(wg.local_memory_size, CL_KERNEL_LOCAL_MEM_SIZE);
	CLX_WG_FIELD(wg.private_memory_size, CL_KERNEL_PRIVATE_MEM_SIZE);
	#endif
	return wg;
	#undef CLX_WG_FIELD
}

#if CL_TARGET_VERSION >= 210
size_t
clx::kernel::max_sub_groups(const device& device) const {
	CLX_BODY_SCALAR(
		::clGetKernelSubGroupInfo,
		size_t,
		device.get(),
		CL_KERNEL_MAX_NUM_SUB_GROUPS,
		0, nullptr
	);
}

size_t
clx::kernel::num_sub_groups(const device& device) const {
	CLX_BODY_SCALAR(
		::clGetKernelSubGroupInfo,
		size_t,
		device.get(),
		CL_KERNEL_COMPILE_NUM_SUB_GROUPS,
		0, nullptr
	);
}

size_t
clx::kernel::max_sub_group_size_210(const device& device, const range& range) const {
	CLX_BODY_SCALAR(
		::clGetKernelSubGroupInfo,
		size_t,
		device.get(),
		CL_KERNEL_MAX_SUB_GROUP_SIZE_FOR_NDRANGE,
		range.dimensions()*sizeof(size_t), range.data()
	);
}

size_t
clx::kernel::num_sub_groups_210(const device& device, const range& range) const {
	CLX_BODY_SCALAR(
		::clGetKernelSubGroupInfo,
		size_t,
		device.get(),
		CL_KERNEL_SUB_GROUP_COUNT_FOR_NDRANGE,
		range.dimensions()*sizeof(size_t), range.data()
	);
}

clx::range
clx::kernel::local_size(const device& device, size_t nsubgroups) const {
	range value{0,0,0};
	CLX_CHECK(
		::clGetKernelSubGroupInfo(
			this->get(),
			device.get(),
			CL_KERNEL_LOCAL_SIZE_FOR_SUB_GROUP_COUNT,
			sizeof(size_t), &nsubgroups,
			sizeof(range), &value, nullptr
		)
	);
	return value;
}

#endif

#if CL_TARGET_VERSION >= 200 && defined(cl_khr_subgroups)

size_t
clx::kernel::max_sub_group_size_khr(const device& device, const range& range) const {
	auto func = CLX_EXTENSION(clGetKernelSubGroupInfoKHR, context().platform());
	CLX_BODY_SCALAR(
		func,
		size_t,
		device.get(),
		CL_KERNEL_MAX_SUB_GROUP_SIZE_FOR_NDRANGE_KHR,
		range.dimensions()*sizeof(size_t), range.data()
	);
}

size_t
clx::kernel::num_sub_groups_khr(const device& device, const range& range) const {
	CLX_BODY_SCALAR(
		::clGetKernelSubGroupInfo,
		size_t,
		device.get(),
		CL_KERNEL_SUB_GROUP_COUNT_FOR_NDRANGE_KHR,
		range.dimensions()*sizeof(size_t), range.data()
	);
}
#endif
