#include <openclx/svm_block>

clx::svm_block::~svm_block() {
	::clSVMFree(this->_context.get(), this->_ptr);
}
