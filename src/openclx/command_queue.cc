#include <openclx/bits/macros>
#include <openclx/command_queue>
#include <openclx/context>
#include <openclx/device>

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
