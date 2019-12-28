#include <openclx/bits/macros>
#include <openclx/downcast>
#include <openclx/intel/context>

#if CL_TARGET_VERSION >= 120 && defined(cl_intel_accelerator)
#include <openclx/intel/me_accelerator>
#endif

#if CL_TARGET_VERSION >= 120 && defined(cl_intel_motion_estimation)
clx::intel::me_accelerator
clx::intel::context::motion_estimation_accelerator(const intel::me_descriptor& desc) const {
    auto pf = platform();
    auto func = CLX_EXTENSION(clCreateAcceleratorINTEL, pf);
    int_type ret = 0;
    auto acc = func(
        this->get(), downcast(intel::accelerators::motion_estimation),
        sizeof(desc), &desc, &ret
    );
    CLX_CHECK(ret);
    return intel::me_accelerator(acc, pf);
}
#endif

#if CL_TARGET_VERSION >= 120 && defined(cl_intel_advanced_motion_estimation)
CLX_METHOD_SCALAR(
    clx::intel::context::motion_estimation_version,
    ::clGetContextInfo,
    unsigned_int_type,
    CL_DEVICE_ME_VERSION_INTEL
)
#endif

#if defined(cl_intel_dx9_media_sharing)
clx::intel::memory_object
clx::intel::context::media_surface(
    memory_flags flags,
    intel::surface_type* ptr,
    intel::handle_type handle,
    intel::uint_type plane
) const {
    auto func = CLX_EXTENSION(clCreateFromDX9MediaSurfaceINTEL, platform());
    int_type ret = 0;
    auto mem = func(this->_ptr, downcast(flags), object, handle, plane, &ret);
    CLX_CHECK(ret);
    return intel::memory_object(mem);
}
#endif

#if defined(cl_intel_dx9_media_sharing)
clx::intel::va::memory_object
clx::intel::context::media_surface(
    memory_flags flags,
    intel::va::surface_type* ptr,
    unsigned_int_type plane
) const {
    auto func = CLX_EXTENSION(clCreateFromVA_APIMediaSurfaceINTEL, platform());
    int_type ret = 0;
    auto mem = func(this->_ptr, downcast(flags), object, plane, &ret);
    CLX_CHECK(ret);
    return intel::va::memory_object(mem);
}
#endif
