#include <openclx/bits/macros>
#include <openclx/command_queue>
#include <openclx/context>
#include <openclx/device>

CLX_WARNING_PUSH
CLX_IGNORED_ATTRIBUTES

CLX_METHOD_SCALAR(
	clx::command_queue::context,
	::clGetCommandQueueInfo, CL_QUEUE_CONTEXT,
	::clx::context
)
CLX_METHOD_SCALAR(
	clx::command_queue::device,
	::clGetCommandQueueInfo, CL_QUEUE_DEVICE,
	::clx::device
)
CLX_METHOD_SCALAR(
	clx::command_queue::num_references,
	::clGetCommandQueueInfo, CL_QUEUE_DEVICE,
	unsigned_int_type
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

CLX_WARNING_POP
