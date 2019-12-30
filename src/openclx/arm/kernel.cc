#include <openclx/arm/kernel>
#include <openclx/array_view>
#include <openclx/bits/macros>
#include <openclx/context>

#if CL_TARGET_VERSION >= 120 && defined(cl_arm_shared_virtual_memory)
void
clx::arm::kernel::svm_pointers(const array_view<void*>& pointers) {
    auto func = CLX_EXTENSION(clSetKernelExecInfoARM, context().platform());
    CLX_CHECK(func(
        this->get(), CL_KERNEL_EXEC_INFO_SVM_PTRS_ARM,
        pointers.size()*sizeof(void*), pointers.data()
    ));
}

void
clx::arm::kernel::fine_grain_system_svm_pointers(bool b) {
    auto func = CLX_EXTENSION(clSetKernelExecInfoARM, context().platform());
    bool_type value = static_cast<bool_type>(b);
    CLX_CHECK(func(
        this->get(), CL_KERNEL_EXEC_INFO_SVM_FINE_GRAIN_SYSTEM_ARM,
        sizeof(bool_type), &value
    ));
}
#endif
