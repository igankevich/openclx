#include <openclx/pipe>
#include <openclx/bits/macros>

CLX_METHOD_SCALAR(
	clx::pipe::packet_size,
	::clGetPipeInfo,
	CL_PIPE_PACKET_SIZE,
	unsigned_int_type
)

CLX_METHOD_SCALAR(
	clx::pipe::max_packets,
	::clGetPipeInfo,
	CL_PIPE_PACKET_SIZE,
	unsigned_int_type
)
