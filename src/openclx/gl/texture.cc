#include <openclx/bits/macros>
#include <openclx/gl/texture>

CLX_METHOD_SCALAR(
    clx::gl::texture::target,
    ::clGetGLTextureInfo,
    enum_type,
    CL_GL_TEXTURE_TARGET
);

CLX_METHOD_SCALAR(
    clx::gl::texture::mipmap_level,
    ::clGetGLTextureInfo,
    int_type,
    CL_GL_MIPMAP_LEVEL
);

#if CL_TARGET_VERSION >= 120
CLX_METHOD_SCALAR(
    clx::gl::texture::num_samples,
    ::clGetGLTextureInfo,
    int_type,
    CL_GL_NUM_SAMPLES
);
#endif
