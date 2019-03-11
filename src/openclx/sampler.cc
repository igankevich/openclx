#include <openclx/bits/macros>
#include <openclx/sampler>
#include <openclx/context>

CLX_GET_SCALAR5(
	clx::sampler::num_references,
	::clGetSamplerInfo,
	CL_SAMPLER_REFERENCE_COUNT,
	unsigned_int_type,
	unsigned_int_type
)

CLX_GET_SCALAR5(
	clx::sampler::context,
	::clGetSamplerInfo,
	CL_SAMPLER_CONTEXT,
	context_type,
	::clx::context
)

CLX_GET_SCALAR5(
	clx::sampler::normalised_coordinates,
	::clGetSamplerInfo,
	CL_SAMPLER_NORMALIZED_COORDS,
	bool_type,
	bool
)

CLX_GET_STRUCT4(
	clx::sampler::addressing_mode,
	::clGetSamplerInfo,
	CL_SAMPLER_ADDRESSING_MODE,
	::clx::addressing_mode
)

CLX_GET_STRUCT4(
	clx::sampler::filter_mode,
	::clGetSamplerInfo,
	CL_SAMPLER_FILTER_MODE,
	::clx::filter_mode
)
