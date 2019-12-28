#include <openclx/binary>
#include <openclx/bits/macros>
#include <openclx/buffer>
#include <openclx/command_queue>
#include <openclx/context>
#include <openclx/device>
#include <openclx/downcast>
#include <openclx/event>
#include <openclx/extensions>
#include <openclx/gl/buffer>
#include <openclx/gl/texture>
#include <openclx/image>
#include <openclx/pipe>
#include <openclx/platform>
#include <openclx/program>
#include <openclx/sampler>
#include <openclx/sampler_properties>
#include <openclx/svm_block>

CLX_METHOD_ARRAY(
    clx::context::devices,
    ::clGetContextInfo, CL_CONTEXT_DEVICES,
    device
)

#if CL_TARGET_VERSION >= 200
std::vector<clx::command_queue>
clx::context::command_queues_200(const command_queue_properties& prop) const {
    const auto& devices = this->devices();
    std::vector<command_queue> result;
    result.reserve(devices.size());
    for (const auto& device : devices) {
        result.emplace_back(device.queue_200(this->_ptr, prop));
    }
    return result;
}
#endif

#if CL_TARGET_VERSION <= 120 || defined(CL_USE_DEPRECATED_OPENCL_1_2_APIS)
std::vector<clx::command_queue>
clx::context::command_queues_100(const command_queue_properties& prop) const {
    const auto& devices = this->devices();
    std::vector<command_queue> result;
    result.reserve(devices.size());
    for (const auto& device : devices) {
        result.emplace_back(device.queue_100(this->_ptr, prop));
    }
    return result;
}
#endif

clx::program
clx::context::program(const std::string& src) const {
    const char* string = src.data();
    size_t length = src.length();
    int_type ret = 0;
    auto prg = ::clCreateProgramWithSource(
        this->_ptr,
        1,
        &string,
        &length,
        &ret
    );
    CLX_CHECK(ret);
    return static_cast<::clx::program>(prg);
}

clx::program
clx::context::program(const array_view<binary>& binaries) const {
    const auto& devices = this->devices();
    if (devices.size() != binaries.size()) {
        throw std::invalid_argument("different number of binaries and devices");
    }
    std::vector<size_t> sizes;
    sizes.reserve(binaries.size());
    std::vector<const unsigned char*> data;
    data.reserve(binaries.size());
    for (const auto& bin : binaries) {
        sizes.emplace_back(bin.size());
        data.emplace_back(bin.data());
    }
    int_type ret = 0;
    std::vector<errc> status(binaries.size());
    auto prg = ::clCreateProgramWithBinary(
        this->_ptr,
        devices.size(), downcast(devices.data()), sizes.data(),
        data.data(), downcast(status.data()),
        &ret
    );
    CLX_CHECK(ret);
    for (const auto& st : status) {
        if (st != errc::success) {
            CLX_THROW(st);
        }
    }
    return static_cast<::clx::program>(prg);
}

#if CL_TARGET_VERSION >= 200
clx::program
clx::context::program(const intermediate_language& il) const {
    int_type ret = 0;
    auto prg = ::clCreateProgramWithIL(this->_ptr, il.data(), il.size(), &ret);
    CLX_CHECK(ret);
    return static_cast<::clx::program>(prg);
}
#endif

#if CL_TARGET_VERSION >= 120
clx::program
clx::context::builtin_program(const std::string& names) const {
    const auto& devices = this->devices();
    int_type ret = 0;
    auto prg = ::clCreateProgramWithBuiltInKernels(
        this->_ptr, devices.size(), downcast(devices.data()),
        names.data(), &ret
    );
    CLX_CHECK(ret);
    return static_cast<::clx::program>(prg);
}
#endif

#if CL_TARGET_VERSION >= 120
clx::header
clx::context::header(const std::string& name, const std::string& src) const {
    return {this->program(src), name};
}
#endif

std::vector<clx::image_format>
clx::context::image_formats(memory_flags flags, memory_objects type) const {
    std::vector<image_format> result(4096 / sizeof(image_format));
    int_type ret = 0;
    bool success = false;
    unsigned_int_type actual_size = 0;
    while (!success) {
        ret = ::clGetSupportedImageFormats(
            this->_ptr, downcast(flags), downcast(type),
            result.size(), downcast(result.data()), &actual_size
        );
        result.resize(actual_size);
        if (errc(ret) != errc::invalid_value && result.size() <= actual_size) {
            CLX_CHECK(ret);
            success = true;
        }
    }
    return result;
}

CLX_METHOD_ARRAY(
    clx::context::properties,
    ::clGetContextInfo, CL_CONTEXT_PROPERTIES,
    context_properties_type
);

#if defined(cl_khr_gl_sharing)
clx::device
clx::context::gl_device() const {
    const auto& prop = this->properties();
    auto func = CLX_EXTENSION(clGetGLContextInfoKHR, platform());
    device_type value = nullptr;
    CLX_CHECK(func(
        prop.data(),
        CL_CURRENT_DEVICE_FOR_GL_CONTEXT_KHR,
        sizeof(device_type),
        &value,
        nullptr
    ));
    return static_cast<device>(value);
}
#endif

#if defined(cl_khr_gl_sharing)
std::vector<clx::device>
clx::context::gl_available_devices() const {
    const auto& prop = this->properties();
    auto func = CLX_EXTENSION(clGetGLContextInfoKHR, platform());
    return ::clx::bits::body_array<device>(
        reinterpret_cast<bits::info_type>(func),
        const_cast<context_properties_type*>(prop.data()),
        CL_DEVICES_FOR_GL_CONTEXT_KHR
    );
}
#endif

CLX_METHOD_SCALAR(
    clx::context::num_references,
    ::clGetContextInfo,
    unsigned_int_type,
    CL_CONTEXT_REFERENCE_COUNT
)

#if CL_TARGET_VERSION >= 120
CLX_METHOD_SCALAR(
    clx::context::num_devices,
    ::clGetContextInfo,
    unsigned_int_type,
    CL_CONTEXT_NUM_DEVICES
)
#endif

#if CL_TARGET_VERSION >= 120
clx::image
clx::context::image_120(
    memory_flags flags,
    const image_format& format,
    const image_descriptor& descriptor,
    void* host_pointer
) const {
    int_type ret = 0;
    memory_type img = nullptr;
    img = ::clCreateImage(
        this->_ptr,
        static_cast<memory_flags_type>(flags),
        &format,
        &descriptor,
        host_pointer,
        &ret
    );
    CLX_CHECK(ret);
    return static_cast<::clx::image>(img);
}
#endif

#if CL_TARGET_VERSION <= 110 || defined(CL_USE_DEPRECATED_OPENCL_1_1_APIS)
clx::image
clx::context::image_100(
    memory_flags flags,
    const image_format& format,
    const image_descriptor& descriptor,
    void* host_pointer
) const {
    int_type ret = 0;
    memory_type img = nullptr;
    if (descriptor.depth() == 0) {
        img = ::clCreateImage2D(
            this->_ptr,
            static_cast<memory_flags_type>(flags),
            &format,
            descriptor.width(),
            descriptor.height(),
            descriptor.row_pitch(),
            host_pointer,
            &ret
        );
    } else {
        img = ::clCreateImage3D(
            this->_ptr,
            static_cast<memory_flags_type>(flags),
            &format,
            descriptor.width(),
            descriptor.height(),
            descriptor.depth(),
            descriptor.row_pitch(),
            descriptor.slice_pitch(),
            host_pointer,
            &ret
        );
    }
    CLX_CHECK(ret);
    return static_cast<::clx::image>(img);
}
#endif

#if CL_TARGET_VERSION >= 110
clx::event
clx::context::event() const {
    int_type ret = 0;
    auto ev = ::clCreateUserEvent(this->_ptr, &ret);
    CLX_CHECK(ret);
    return static_cast<::clx::event>(ev);
}
#endif

#if CL_TARGET_VERSION >= 110 && defined(cl_khr_gl_sharing)
clx::event
clx::context::gl_event(gl::sync_type name) const {
    int_type ret = 0;
    auto ev = ::clCreateEventFromGLsyncKHR(this->_ptr, name, &ret);
    CLX_CHECK(ret);
    return static_cast<::clx::event>(ev);
}
#endif

#if defined(cl_khr_egl_event)
clx::event
clx::context::egl_event(egl::sync_type name, egl::display_type display) const {
    auto func = CLX_EXTENSION(clCreateEventFromEGLSyncKHR, platform());
    int_type ret = 0;
    auto ev = func(this->_ptr, name, display, &ret);
    CLX_CHECK(ret);
    return static_cast<::clx::event>(ev);
}
#endif

#if defined(cl_khr_egl_image)
clx::egl::image
clx::context::egl_image(memory_flags flags, const egl::image_properties& prop) const {
    auto func = CLX_EXTENSION(clCreateFromEGLImageKHR, platform());
    int_type ret = 0;
    auto img = func(
        this->_ptr,
        prop.display(), prop.name(), downcast(flags),
        prop.commit(), &ret
    );
    CLX_CHECK(ret);
    return static_cast<egl::image>(img);
}
#endif

#if defined(cl_arm_import_memory)
::clx::buffer import(
    memory_flags flags,
    const arm::import_properties& prop,
    void* memory,
    size_t size
) const {
    auto func = CLX_EXTENSION(clImportMemoryARM, platform());
    int_type ret = 0;
    auto buf = func(this->_ptr, downcast(flags), prop.data(), memory, size, &ret);
    CLX_CHECK(ret);
    return static_cast<::clx::buffer>(buf);
}
#endif

clx::buffer
clx::context::buffer(
    memory_flags flags,
    size_t size,
    void* host_pointer
) const {
    int_type ret = 0;
    auto buf =
        ::clCreateBuffer(
            this->_ptr,
            static_cast<memory_flags_type>(flags),
            size,
            host_pointer,
            &ret
        );
    CLX_CHECK(ret);
    return static_cast<::clx::buffer>(buf);
}

#if CL_TARGET_VERSION >= 200
clx::sampler
clx::context::sampler_200(const sampler_properties& prop) const {
    int_type ret = 0;
    auto props = prop(platform().extensions());
    auto sm = ::clCreateSamplerWithProperties(this->_ptr, props.data(), &ret);
    CLX_CHECK(ret);
    return static_cast<::clx::sampler>(sm);
}
#endif

#if CL_TARGET_VERSION <= 120 || defined(CL_USE_DEPRECATED_OPENCL_1_2_APIS)
clx::sampler
clx::context::sampler_100(const sampler_properties& prop) const {
    int_type ret = 0;
    auto sm = ::clCreateSampler(
        this->_ptr,
        static_cast<bool_type>(prop.normalised()),
        static_cast<addressing_mode_type>(prop.get_addressing_mode()),
        static_cast<filter_mode_type>(prop.get_filter_mode()),
        &ret
    );
    CLX_CHECK(ret);
    return static_cast<::clx::sampler>(sm);
}
#endif

#if CL_TARGET_VERSION >= 200
clx::pipe
clx::context::pipe(
    memory_flags flags,
    unsigned_int_type packet_size,
    unsigned_int_type max_packets
) const {
    int_type ret = 0;
    auto p = ::clCreatePipe(
        this->_ptr,
        static_cast<memory_flags_type>(flags),
        packet_size,
        max_packets,
        nullptr,
        &ret
    );
    CLX_CHECK(ret);
    return static_cast<::clx::pipe>(p);
}
#endif

#if CL_TARGET_VERSION >= 200
clx::svm_block
clx::context::shared_memory_200(
    svm_flags flags,
    size_t size,
    size_t alignment
) const {
    auto ptr = ::clSVMAlloc(this->_ptr, downcast(flags), size, alignment);
    if (!ptr) { throw std::bad_alloc{}; }
    return svm_block{ptr, size};
}
#endif

#if CL_TARGET_VERSION >= 120 && defined(cl_arm_shared_virtual_memory)
clx::svm_block
clx::context::shared_memory_arm(
    svm_flags flags,
    size_t size,
    size_t alignment
) const {
    auto func = CLX_EXTENSION(clSVMAllocARM, platform());
    auto ptr = func(this->_ptr, downcast(flags), size, alignment);
    if (!ptr) { throw std::bad_alloc{}; }
    return svm_block{ptr, size};
}
#endif

clx::platform
clx::context::platform() const {
    if (this->_platform.get()) { return this->_platform; }
    const auto& prop = this->properties();
    const auto nprops = prop.size();
    for (size_t i=0; i<nprops; ++i) {
        if (prop[i] == CL_CONTEXT_PLATFORM && i < nprops-1) {
            this->_platform = ::clx::platform(reinterpret_cast<platform_type>(prop[i+1]));
            break;
        }
    }
    return this->_platform;
}


#if defined(cl_khr_gl_sharing)
clx::gl::buffer
clx::context::gl_buffer(memory_flags flags, gl::unsigned_int_type name) const {
    int_type ret = 0;
    auto buf = ::clCreateFromGLBuffer(this->_ptr, downcast(flags), name, &ret);
    CLX_CHECK(ret);
    return static_cast<gl::buffer>(buf);
}
#endif

#if defined(cl_khr_gl_sharing)
clx::gl::buffer
clx::context::gl_renderbuffer(memory_flags flags, gl::unsigned_int_type name) const {
    int_type ret = 0;
    auto buf = ::clCreateFromGLRenderbuffer(this->_ptr, downcast(flags), name, &ret);
    CLX_CHECK(ret);
    return static_cast<gl::buffer>(buf);
}
#endif

#if CL_TARGET_VERSION >= 120 && defined(cl_khr_gl_sharing)
clx::gl::texture
clx::context::gl_texture(
    memory_flags flags,
    gl::enum_type target,
    gl::int_type miplevel,
    gl::unsigned_int_type name
) const {
    int_type ret = 0;
    auto tex = ::clCreateFromGLTexture(
        this->_ptr, downcast(flags),
        target, miplevel, name, &ret
    );
    CLX_CHECK(ret);
    return static_cast<gl::texture>(tex);
}
#endif

#if defined(cl_khr_gl_sharing)
#if (CL_TARGET_VERSION <= 110 || defined(CL_USE_DEPRECATED_OPENCL_1_1_APIS))
clx::gl::texture
clx::context::gl_texture_2d_100(
    memory_flags flags,
    gl::enum_type target,
    gl::int_type miplevel,
    gl::unsigned_int_type name
) const {
    int_type ret = 0;
    auto tex = ::clCreateFromGLTexture2D(
        this->_ptr, downcast(flags),
        target, miplevel, name, &ret
    );
    CLX_CHECK(ret);
    return static_cast<gl::texture>(tex);
}

clx::gl::texture
clx::context::gl_texture_3d_100(
    memory_flags flags,
    gl::enum_type target,
    gl::int_type miplevel,
    gl::unsigned_int_type name
) const {
    int_type ret = 0;
    auto tex = ::clCreateFromGLTexture3D(
        this->_ptr, downcast(flags),
        target, miplevel, name, &ret
    );
    CLX_CHECK(ret);
    return static_cast<gl::texture>(tex);
}
#endif
#endif

#if CL_TARGET_VERSION >= 120 && defined(cl_khr_terminate_context)
void
clx::context::terminate() {
    auto func = CLX_EXTENSION(clTerminateContextKHR, platform());
    CLX_CHECK(func(this->_ptr));
}
#endif

#if defined(cl_khr_dx9_media_sharing)
clx::d3d9::media_surface
clx::context::media_surface(memory_flags flags, d3d9::adapter_type adapter, void* ptr) const {
    auto func = CLX_EXTENSION(clCreateFromDX9MediaSurfaceKHR, platform());
    int_type ret = 0;
    auto mem = func(this->_ptr, downcast(flags), downcast(adapter), ptr, &ret);
    CLX_CHECK(ret);
    return d3d9::media_surface(mem);
}
#endif

#if defined(cl_khr_d3d10_sharing)
clx::d3d10::memory_object
clx::context::buffer(memory_flags flags, d3d10::buffer_type* ptr) const {
    auto func = CLX_EXTENSION(clCreateFromD3D10BufferKHR, platform());
    int_type ret = 0;
    auto mem = func(this->_ptr, downcast(flags), ptr, &ret);
    CLX_CHECK(ret);
    return d3d10::memory_object(mem);
}
clx::d3d10::memory_object
clx::context::buffer(memory_flags flags, d3d10::texture2d_type* ptr) const {
    auto func = CLX_EXTENSION(clCreateFromD3D10Texture2DKHR, platform());
    int_type ret = 0;
    auto mem = func(this->_ptr, downcast(flags), ptr, &ret);
    CLX_CHECK(ret);
    return d3d10::memory_object(mem);
}
clx::d3d10::memory_object
clx::context::buffer(memory_flags flags, d3d10::texture3d_type* ptr) const {
    auto func = CLX_EXTENSION(clCreateFromD3D10Texture3DKHR, platform());
    int_type ret = 0;
    auto mem = func(this->_ptr, downcast(flags), ptr, &ret);
    CLX_CHECK(ret);
    return d3d10::memory_object(mem);
}
CLX_METHOD_BOOL(
    clx::context::d3d10_prefer_shared_resources,
    ::clGetContextInfo,
    CL_CONTEXT_D3D10_PREFER_SHARED_RESOURCES_KHR
);
#endif

#if CL_TARGET_VERSION >= 120 && defined(cl_khr_d3d11_sharing)
clx::d3d11::memory_object
clx::context::buffer(memory_flags flags, d3d11::buffer_type* ptr) const {
    auto func = CLX_EXTENSION(clCreateFromD3D11BufferKHR, platform());
    int_type ret = 0;
    auto mem = func(this->_ptr, downcast(flags), ptr, &ret);
    CLX_CHECK(ret);
    return d3d11::memory_object(mem);
}
clx::d3d11::memory_object
clx::context::buffer(memory_flags flags, d3d11::texture2d_type* ptr) const {
    auto func = CLX_EXTENSION(clCreateFromD3D11Texture2DKHR, platform());
    int_type ret = 0;
    auto mem = func(this->_ptr, downcast(flags), ptr, &ret);
    CLX_CHECK(ret);
    return d3d11::memory_object(mem);
}
clx::d3d11::memory_object
clx::context::buffer(memory_flags flags, d3d11::texture3d_type* ptr) const {
    auto func = CLX_EXTENSION(clCreateFromD3D11Texture3DKHR, platform());
    int_type ret = 0;
    auto mem = func(this->_ptr, downcast(flags), ptr, &ret);
    CLX_CHECK(ret);
    return d3d11::memory_object(mem);
}
CLX_METHOD_BOOL(
    clx::context::d3d11_prefer_shared_resources,
    ::clGetContextInfo,
    CL_CONTEXT_D3D11_PREFER_SHARED_RESOURCES_KHR
);
#endif
