#include <openclx/bits/macros>
#include <openclx/intel/device>
#include <openclx/intel/kernel>

#if CL_TARGET_VERSION >= 120 && defined(cl_intel_required_subgroup_size)
clx::intel::work_group
clx::intel::kernel::work_group(const device& dev) const {
    ::clx::intel::work_group wg{};
    CLX_WG_FIELD(wg.size, CL_KERNEL_WORK_GROUP_SIZE);
    CLX_WG_FIELD(wg.size_hint, CL_KERNEL_COMPILE_WORK_GROUP_SIZE);
    CLX_WG_FIELD(wg.size_multiple, CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE);
    CLX_WG_FIELD(wg.local_memory_size, CL_KERNEL_LOCAL_MEM_SIZE);
    CLX_WG_FIELD(wg.private_memory_size, CL_KERNEL_PRIVATE_MEM_SIZE);
    CLX_WG_FIELD(wg.spill_memory_size, CL_KERNEL_SPILL_MEM_SIZE_INTEL);
    return wg;
}
#endif

#if CL_TARGET_VERSION >= 210 && \
    defined(cl_intel_required_subgroup_size) && \
    defined(CLX_HAVE_clGetKernelSubGroupInfo)
size_t
clx::intel::kernel::sub_groups_size(const device& device) const {
    size_t result = 0;
    CLX_BODY_SCALAR(
        ::clGetKernelSubGroupInfo,
        size_t,
        device.get(),
        CL_KERNEL_COMPILE_SUB_GROUP_SIZE_INTEL,
        sizeof(size_t), &result
    );
    return result;
}
#endif

