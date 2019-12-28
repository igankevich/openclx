#include <openclx/array_view>
#include <openclx/bits/macros>
#include <openclx/command_queue>
#include <openclx/context>
#include <openclx/downcast>
#include <openclx/event>

CLX_METHOD_SCALAR(
    clx::event::queue,
    ::clGetEventInfo,
    ::clx::command_queue,
    CL_EVENT_COMMAND_QUEUE
);

CLX_METHOD_SCALAR(
    clx::event::command,
    ::clGetEventInfo,
    ::clx::command,
    CL_EVENT_COMMAND_TYPE
);

CLX_METHOD_SCALAR(
    clx::event::status,
    ::clGetEventInfo,
    ::clx::execution_status,
    CL_EVENT_COMMAND_EXECUTION_STATUS
);

CLX_METHOD_SCALAR(
    clx::event::num_references,
    ::clGetEventInfo,
    unsigned_int_type,
    CL_EVENT_REFERENCE_COUNT
);

CLX_METHOD_SCALAR(
    clx::event::time_queued,
    ::clGetEventProfilingInfo,
    nanoseconds,
    CL_PROFILING_COMMAND_QUEUED
);

CLX_METHOD_SCALAR(
    clx::event::time_submitted,
    ::clGetEventProfilingInfo,
    nanoseconds,
    CL_PROFILING_COMMAND_SUBMIT
);

CLX_METHOD_SCALAR(
    clx::event::time_started,
    ::clGetEventProfilingInfo,
    nanoseconds,
    CL_PROFILING_COMMAND_START
);

CLX_METHOD_SCALAR(
    clx::event::time_ended,
    ::clGetEventProfilingInfo,
    nanoseconds,
    CL_PROFILING_COMMAND_END
);

CLX_METHOD_SCALAR(
    clx::event::time_completed,
    ::clGetEventProfilingInfo,
    nanoseconds,
    CL_PROFILING_COMMAND_COMPLETE
);

void
clx::wait(const array_view<event>& events) {
    CLX_CHECK(::clWaitForEvents(events.size(), downcast(events.data())));
}

