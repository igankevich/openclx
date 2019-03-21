#include <openclx/bits/macros>
#include <openclx/sampler>
#include <openclx/context>

CLX_METHOD_SCALAR(
	clx::sampler::num_references,
	::clGetSamplerInfo,
	CL_SAMPLER_REFERENCE_COUNT,
	unsigned_int_type
)

CLX_METHOD_SCALAR(
	clx::sampler::context,
	::clGetSamplerInfo,
	CL_SAMPLER_CONTEXT,
	::clx::context
)

CLX_METHOD_BOOL(
	clx::sampler::normalised_coordinates,
	::clGetSamplerInfo,
	CL_SAMPLER_NORMALIZED_COORDS
)

CLX_METHOD_SCALAR(
	clx::sampler::addressing_mode,
	::clGetSamplerInfo,
	CL_SAMPLER_ADDRESSING_MODE,
	::clx::addressing_mode
)

CLX_METHOD_SCALAR(
	clx::sampler::filter_mode,
	::clGetSamplerInfo,
	CL_SAMPLER_FILTER_MODE,
	::clx::filter_mode
)
