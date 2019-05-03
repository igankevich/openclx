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
	#if defined(cl_intel_driver_diagnostics)
	if (extensions("cl_intel_driver_diagnostics")) {
		prop.push_back(CL_CONTEXT_SHOW_DIAGNOSTICS_INTEL);
		prop.push_back(context_properties_type(diagnostics()));
	}
	#endif
	#if defined(cl_khr_dx9_media_sharing)
	if (extensions("cl_khr_dx9_media_sharing")) {
		if (d3d9_adapter()) {
			prop.push_back(CL_CONTEXT_ADAPTER_D3D9_KHR);
			prop.push_back(context_properties_type(d3d9_adapter()));
		}
		if (d3d9ex_adapter()) {
			prop.push_back(CL_CONTEXT_ADAPTER_D3D9EX_KHR);
			prop.push_back(context_properties_type(d3d9ex_adapter()));
		}
		if (dxva_adapter()) {
			prop.push_back(CL_CONTEXT_ADAPTER_DXVA_KHR);
			prop.push_back(context_properties_type(dxva_adapter()));
		}
	}
	#if defined(cl_intel_dx9_media_sharing)
	else
	#endif
	#endif
	#if defined(cl_intel_dx9_media_sharing)
	if (extensions("cl_khr_dx9_media_sharing")) {
		if (d3d9_adapter()) {
			prop.push_back(CL_CONTEXT_D3D9_DEVICE_INTEL);
			prop.push_back(context_properties_type(d3d9_adapter()));
		}
		if (d3d9ex_adapter()) {
			prop.push_back(CL_CONTEXT_D3D9EX_DEVICE_INTEL);
			prop.push_back(context_properties_type(d3d9ex_adapter()));
		}
		if (dxva_adapter()) {
			prop.push_back(CL_CONTEXT_DXVA_DEVICE_INTEL);
			prop.push_back(context_properties_type(dxva_adapter()));
		}
	}
	#endif
	#if defined(cl_khr_d3d10_sharing)
	if (extensions("cl_khr_d3d10_sharing")) {
		if (d3d10_device()) {
			prop.push_back(CL_CONTEXT_D3D10_DEVICE_KHR);
			prop.push_back(context_properties_type(d3d10_device()));
		}
	}
	#endif
	#if defined(cl_khr_d3d11_sharing)
	if (extensions("cl_khr_d3d11_sharing")) {
		if (d3d11_device()) {
			prop.push_back(CL_CONTEXT_D3D10_DEVICE_KHR);
			prop.push_back(context_properties_type(d3d11_device()));
		}
	}
	#endif
	#if defined(cl_intel_va_api_media_sharing)
	if (extensions("cl_intel_va_api_media_sharing")) {
		if (va_display()) {
			prop.push_back(CL_CONTEXT_VA_API_DISPLAY_INTEL);
			prop.push_back(context_properties_type(va_display()));
		}
	}
	#endif
	return prop;
}

