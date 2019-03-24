#include <openclx/bits/macros>
#include <openclx/command_queue>
#include <openclx/context>
#include <openclx/downcast>
#include <openclx/event>

CLX_METHOD_SCALAR(
	clx::event::queue,
	::clGetEventInfo,
	CL_EVENT_COMMAND_QUEUE,
	::clx::command_queue
);

CLX_METHOD_SCALAR(
	clx::event::command,
	::clGetEventInfo,
	CL_EVENT_COMMAND_TYPE,
	::clx::command
);

CLX_METHOD_SCALAR(
	clx::event::status,
	::clGetEventInfo,
	CL_EVENT_COMMAND_EXECUTION_STATUS,
	::clx::execution_status
);

CLX_METHOD_SCALAR(
	clx::event::num_references,
	::clGetEventInfo,
	CL_EVENT_REFERENCE_COUNT,
	unsigned_int_type
);

CLX_METHOD_SCALAR(
	clx::event::time_queued,
	::clGetEventProfilingInfo,
	CL_PROFILING_COMMAND_QUEUED,
	nanoseconds
);

CLX_METHOD_SCALAR(
	clx::event::time_submitted,
	::clGetEventProfilingInfo,
	CL_PROFILING_COMMAND_SUBMIT,
	nanoseconds
);

CLX_METHOD_SCALAR(
	clx::event::time_started,
	::clGetEventProfilingInfo,
	CL_PROFILING_COMMAND_START,
	nanoseconds
);

CLX_METHOD_SCALAR(
	clx::event::time_ended,
	::clGetEventProfilingInfo,
	CL_PROFILING_COMMAND_END,
	nanoseconds
);

CLX_METHOD_SCALAR(
	clx::event::time_completed,
	::clGetEventProfilingInfo,
	CL_PROFILING_COMMAND_COMPLETE,
	nanoseconds
);

void
clx::wait(const std::vector<event>& events) {
	CLX_CHECK(::clWaitForEvents(events.size(), downcast(events.data())));
}

