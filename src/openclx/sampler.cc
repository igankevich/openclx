#include <openclx/bits/macros>
#include <openclx/sampler>
#include <openclx/context>

CLX_METHOD_SCALAR(
	clx::sampler::num_references,
	::clGetSamplerInfo,
	unsigned_int_type,
	CL_SAMPLER_REFERENCE_COUNT
)

CLX_METHOD_SCALAR(
	clx::sampler::context,
	::clGetSamplerInfo,
	::clx::context,
	CL_SAMPLER_CONTEXT
)

CLX_METHOD_BOOL(
	clx::sampler::normalised_coordinates,
	::clGetSamplerInfo,
	CL_SAMPLER_NORMALIZED_COORDS
)

CLX_METHOD_SCALAR(
	clx::sampler::addressing_mode,
	::clGetSamplerInfo,
	::clx::addressing_mode,
	CL_SAMPLER_ADDRESSING_MODE
)

CLX_METHOD_SCALAR(
	clx::sampler::filter_mode,
	::clGetSamplerInfo,
	::clx::filter_mode,
	CL_SAMPLER_FILTER_MODE
)
