#include <openclx/bits/macros>
#include <openclx/kernel_argument>

#if CL_TARGET_VERSION >= 120

CLX_METHOD_STRING(
    clx::kernel_argument::name,
    ::clGetKernelArgInfo,
    this->_index,
    CL_KERNEL_ARG_NAME
);

CLX_METHOD_STRING(
    clx::kernel_argument::type_name,
    ::clGetKernelArgInfo,
    this->_index,
    CL_KERNEL_ARG_TYPE_NAME
);

CLX_METHOD_SCALAR(
    clx::kernel_argument::type,
    ::clGetKernelArgInfo,
    argument_type_qualifiers,
    this->_index,
    CL_KERNEL_ARG_TYPE_QUALIFIER
);

CLX_METHOD_SCALAR(
    clx::kernel_argument::address,
    ::clGetKernelArgInfo,
    address_qualifiers,
    this->_index,
    CL_KERNEL_ARG_ADDRESS_QUALIFIER
);

CLX_METHOD_SCALAR(
    clx::kernel_argument::access,
    ::clGetKernelArgInfo,
    access_qualifiers,
    this->_index,
    CL_KERNEL_ARG_ADDRESS_QUALIFIER
);

#endif
