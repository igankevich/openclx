#include <openclx/bits/macros>
#include <openclx/gl/buffer>

clx::gl::memory_objects
clx::gl::buffer::type() const {
    memory_objects_type value;
    CLX_CHECK(::clGetGLObjectInfo(this->_ptr, &value, nullptr));
    return memory_objects(value);
}

clx::gl::unsigned_int_type
clx::gl::buffer::name() const {
    unsigned_int_type value;
    CLX_CHECK(::clGetGLObjectInfo(this->_ptr, nullptr, &value));
    return value;
}

