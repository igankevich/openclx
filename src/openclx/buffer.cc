#include <openclx/buffer>
#include <openclx/error>

#if CL_TARGET_VERSION >= 110
clx::buffer
clx::buffer::subbuffer(size_t origin, size_t size, memory_flags flags) {
    int_type ret = 0;
    buffer_region_type region{origin, size};
    auto buf = ::clCreateSubBuffer(
        this->_ptr,
        static_cast<memory_flags_type>(flags),
        CL_BUFFER_CREATE_TYPE_REGION,
        &region,
        &ret
    );
    CLX_CHECK(ret);
    return static_cast<buffer>(buf);
}
#endif
