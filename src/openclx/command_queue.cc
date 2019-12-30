#include <openclx/bits/macros>
#include <openclx/command_queue>
#include <openclx/context>
#include <openclx/device>
#include <openclx/downcast>
#include <openclx/extensions>

CLX_METHOD_SCALAR(
    clx::command_queue::context,
    ::clGetCommandQueueInfo,
    ::clx::context,
    CL_QUEUE_CONTEXT
)

CLX_METHOD_SCALAR(
    clx::command_queue::device,
    ::clGetCommandQueueInfo,
    ::clx::device,
    CL_QUEUE_DEVICE
)

CLX_METHOD_SCALAR(
    clx::command_queue::num_references,
    ::clGetCommandQueueInfo,
    unsigned_int_type,
    CL_QUEUE_DEVICE
)

CLX_METHOD_ARRAY(
    clx::command_queue::properties,
    ::clGetCommandQueueInfo, CL_QUEUE_PROPERTIES,
    command_queue_flags_type
);

#if CL_TARGET_VERSION >= 210
void
clx::command_queue::make_default() {
    CLX_CHECK(::clSetDefaultDeviceCommandQueue(
        this->context().get(),
        this->device().get(),
        this->_ptr
    ));
}
#endif

#if CL_TARGET_VERSION >= 200
clx::command_queue_type
clx::command_queue::make_200(
    context_type ctx,
    ::clx::device dev,
    const command_queue_properties& prop
) {
    const auto& props = prop(dev.extensions());
    int_type ret = 0;
    auto result = ::clCreateCommandQueueWithProperties(ctx, dev.get(), props.data(), &ret);
    CLX_CHECK(ret);
    return result;
}
#endif

#if CL_TARGET_VERSION <= 120 || defined(CL_USE_DEPRECATED_OPENCL_1_2_APIS)
clx::command_queue_type
clx::command_queue::make_100(
    context_type ctx,
    ::clx::device dev,
    const command_queue_properties& prop
) {
    int_type ret = 0;
    #if defined(cl_khr_create_command_queue)
    const auto& ext = dev.extensions();
    command_queue_type result;
    if (ext("cl_khr_create_command_queue")) {
        auto func = CLX_EXTENSION(clCreateCommandQueueWithPropertiesKHR, platform());
        const auto& props = prop(dev.extensions());
        result = func(ctx, dev.get(), props.data(), &ret);
    } else
    #endif
    { result = ::clCreateCommandQueue(ctx, dev.get(), downcast(prop.flags()), &ret); }
    CLX_CHECK(ret);
    return result;
}
#endif

