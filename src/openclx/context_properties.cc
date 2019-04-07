#include <openclx/context_properties>

std::vector<clx::context_properties_type>&
clx::operator<<(
	std::vector<context_properties_type>& out,
	const context_properties& rhs
) {
	#if defined(cl_khr_terminate_context)
	out.push_back(context_properties_type(rhs.can_be_terminated()));
	#endif
	return out;
}


