#include <openclx/context_properties>
#include <openclx/platform>

std::vector<clx::context_properties_type>
clx::context_properties::operator()(const platform& platform) const {
	std::vector<context_properties_type> prop;
	prop.reserve(29);
	const auto& extensions = platform.extensions();
	auto supports =
		[&extensions] (const char* n) {
			return extensions.find(n) != std::string::npos;
		};
	if (supports("cl_khr_terminate_context")) {
		prop.push_back(context_properties_type(CL_CONTEXT_TERMINATE_KHR));
		prop.push_back(context_properties_type(can_be_terminated()));
	}
	if (supports("cl_arm_printf")) {
		if (printf_callback()) {
			prop.push_back(context_properties_type(CL_PRINTF_CALLBACK_ARM));
			prop.push_back(context_properties_type(printf_callback()));
		}
		if (printf_buffer_size() > 0) {
			prop.push_back(context_properties_type(CL_PRINTF_BUFFERSIZE_ARM));
			prop.push_back(context_properties_type(printf_buffer_size()));
		}
	}
	return prop;
}

