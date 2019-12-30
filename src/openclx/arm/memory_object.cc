#include <openclx/arm/memory_object>
#include <openclx/bits/macros>

#if CL_TARGET_VERSION >= 120 && defined(cl_arm_shared_virtual_memory)
CLX_METHOD_BOOL(
    clx::arm::memory_object::uses_svm_pointer,
    ::clGetMemObjectInfo,
    CL_MEM_USES_SVM_POINTER_ARM
);
#endif
