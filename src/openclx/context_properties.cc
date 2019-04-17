#include <openclx/context_properties>
#include <openclx/extensions>
#include <openclx/platform>

std::vector<clx::context_properties_type>
clx::context_properties::operator()(const extensions& extensions) const {
	std::vector<context_properties_type> prop;
	prop.reserve(29);
	if (extensions("cl_khr_terminate_context")) {
		prop.push_back(context_properties_type(CL_CONTEXT_TERMINATE_KHR));
		prop.push_back(context_properties_type(can_be_terminated()));
	}
	if (extensions("cl_arm_printf")) {
		if (printf_callback()) {
			prop.push_back(context_properties_type(CL_PRINTF_CALLBACK_ARM));
			prop.push_back(context_properties_type(printf_callback()));
		}
		if (printf_buffer_size() > 0) {
			prop.push_back(context_properties_type(CL_PRINTF_BUFFERSIZE_ARM));
			prop.push_back(context_properties_type(printf_buffer_size()));
		}
	}
	if (extensions("cl_khr_initialize_memory")) {
		prop.push_back(context_properties_type(CL_CONTEXT_MEMORY_INITIALIZE_KHR));
		prop.push_back(context_properties_type(init_memory()));
	}
	#if defined(cl_khr_gl_sharing)
	if (extensions("cl_khr_gl_sharing")) {
		if (gl_context()) {
			prop.push_back(CL_GL_CONTEXT_KHR);
			prop.push_back(context_properties_type(gl_context()));
			prop.push_back(this->_display);
			prop.push_back(context_properties_type(this->_any_display));
		}
		if (gl_sharegroup()) {
			prop.push_back(CL_CGL_SHAREGROUP_KHR);
			prop.push_back(context_properties_type(gl_sharegroup()));
		}
	}
	#endif
	return prop;
}

