#include <openclx/bits/macros>
#include <openclx/command_queue>
#include <openclx/context>
#include <openclx/device>

CLX_WARNING_PUSH
CLX_IGNORED_ATTRIBUTES

#define CLX_QUEUE_GET_SCALAR4(name, key, type, return_type) \
	auto \
	clx::command_queue::name() const -> return_type { \
		CLX_GET_SCALAR4(::clGetCommandQueueInfo, key, type, return_type) \
	}

CLX_QUEUE_GET_SCALAR4(context, CL_QUEUE_CONTEXT, context_type, ::clx::context)
CLX_QUEUE_GET_SCALAR4(device, CL_QUEUE_DEVICE, device_type, ::clx::device)
CLX_QUEUE_GET_SCALAR4(num_references, CL_QUEUE_DEVICE, unsigned_int_type, unsigned_int_type)

std::vector<clx::command_queue_properties_type>
clx::command_queue::properties() const {
	CLX_GET_ARRAY(
		::clGetCommandQueueInfo,
		CL_QUEUE_PROPERTIES,
		command_queue_properties_type,
		command_queue_properties_type
	);
}

#if CLX_OPENCL_VERSION >= 210
void
clx::command_queue::make_default() {
	CLX_CHECK(::clSetDefaultDeviceCommandQueue(
		this->context().get(),
		this->device().get(),
		this->_ptr
	));
}
#endif

CLX_WARNING_POP
