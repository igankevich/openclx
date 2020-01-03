#include <openclx/bits/macros>
#include <openclx/svm_block>

#if CL_TARGET_VERSION >= 200 || \
    (CL_TARGET_VERSION >= 120 && defined(cl_arm_shared_virtual_memory))
clx::svm_block::~svm_block() {
    #if CL_TARGET_VERSION >= 200
    ::clSVMFree(this->_context.get(), this->_ptr);
    #elif CL_TARGET_VERSION >= 120 && defined(cl_arm_shared_virtual_memory)
    auto func = CLX_EXTENSION(clSVMFreeARM, this->_context.platform());
    func(this->_context.get(), this->_ptr);
    #endif
}
#endif
