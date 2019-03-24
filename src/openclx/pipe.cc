#include <openclx/pipe>
#include <openclx/bits/macros>

CLX_METHOD_SCALAR(
	clx::pipe::packet_size,
	::clGetPipeInfo,
	unsigned_int_type,
	CL_PIPE_PACKET_SIZE
)

CLX_METHOD_SCALAR(
	clx::pipe::max_packets,
	::clGetPipeInfo,
	unsigned_int_type,
	CL_PIPE_PACKET_SIZE
)
