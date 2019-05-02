#include <openclx/intel/me_accelerator>
#include <openclx/bits/macros>

clx::intel::me_descriptor
clx::intel::me_accelerator::descriptor() const {
	auto func = CLX_EXTENSION(clGetAcceleratorInfoINTEL, this->_platform);
	CLX_BODY_SCALAR(func, me_descriptor, CL_ACCELERATOR_DESCRIPTOR_INTEL);
}

