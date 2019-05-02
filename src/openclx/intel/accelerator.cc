#include <openclx/bits/macros>
#include <openclx/context>
#include <openclx/intel/accelerator>

void
clx::intel::accelerator::release() {
	if (this->_ptr) {
		auto func = CLX_EXTENSION(clReleaseAcceleratorINTEL, this->_platform);
		CLX_CHECK(func(this->_ptr));
		this->_ptr = nullptr;
	}
}

void
clx::intel::accelerator::retain() {
	if (this->_ptr) {
		auto func = CLX_EXTENSION(clRetainAcceleratorINTEL, this->_platform);
		CLX_CHECK(func(this->_ptr));
	}
}

clx::context
clx::intel::accelerator::context() const {
	auto func = CLX_EXTENSION(clGetAcceleratorInfoINTEL, this->_platform);
	CLX_BODY_SCALAR(func, ::clx::context, CL_ACCELERATOR_CONTEXT_INTEL);
}

clx::unsigned_int_type
clx::intel::accelerator::num_references() const {
	auto func = CLX_EXTENSION(clGetAcceleratorInfoINTEL, this->_platform);
	CLX_BODY_SCALAR(func, unsigned_int_type, CL_ACCELERATOR_REFERENCE_COUNT_INTEL);
}

clx::intel::accelerators
clx::intel::accelerator::type() const {
	auto func = CLX_EXTENSION(clGetAcceleratorInfoINTEL, this->_platform);
	CLX_BODY_SCALAR(func, accelerators, CL_ACCELERATOR_TYPE_INTEL);
}


