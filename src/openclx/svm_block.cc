#include <openclx/svm_block>

#if CL_TARGET_VERSION >= 200
clx::svm_block::~svm_block() {
	::clSVMFree(this->_context.get(), this->_ptr);
}
#endif
