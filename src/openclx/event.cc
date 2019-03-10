#include <openclx/bits/macros>
#include <openclx/command_queue>
#include <openclx/context>
#include <openclx/event>

clx::event::event(const context& ctx) {
	int_type ret = 0;
	this->_ptr = ::clCreateUserEvent(ctx.get(), &ret);
	CLX_CHECK(ret);
}

CLX_GET_SCALAR5(
	clx::event::queue,
	::clGetEventInfo,
	CL_EVENT_COMMAND_QUEUE,
	command_queue_type,
	::clx::command_queue
);

CLX_GET_SCALAR5(
	clx::event::command,
	::clGetEventInfo,
	CL_EVENT_COMMAND_TYPE,
	command_type,
	::clx::command
);

CLX_GET_SCALAR5(
	clx::event::status,
	::clGetEventInfo,
	CL_EVENT_COMMAND_EXECUTION_STATUS,
	command_status_type,
	::clx::command_status
);

CLX_GET_SCALAR5(
	clx::event::num_references,
	::clGetEventInfo,
	CL_EVENT_REFERENCE_COUNT,
	unsigned_int_type,
	unsigned_int_type
);

CLX_GET_SCALAR5(
	clx::event::time_queued,
	::clGetEventProfilingInfo,
	CL_PROFILING_COMMAND_QUEUED,
	unsigned_long_type,
	nanoseconds
);

CLX_GET_SCALAR5(
	clx::event::time_submitted,
	::clGetEventProfilingInfo,
	CL_PROFILING_COMMAND_SUBMIT,
	unsigned_long_type,
	nanoseconds
);

CLX_GET_SCALAR5(
	clx::event::time_started,
	::clGetEventProfilingInfo,
	CL_PROFILING_COMMAND_START,
	unsigned_long_type,
	nanoseconds
);

CLX_GET_SCALAR5(
	clx::event::time_ended,
	::clGetEventProfilingInfo,
	CL_PROFILING_COMMAND_END,
	unsigned_long_type,
	nanoseconds
);

CLX_GET_SCALAR5(
	clx::event::time_completed,
	::clGetEventProfilingInfo,
	CL_PROFILING_COMMAND_COMPLETE,
	unsigned_long_type,
	nanoseconds
);

