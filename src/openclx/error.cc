#include <sstream>

#include <openclx/error>

const char*
clx::to_string(errc rhs) {
	switch (rhs) {
		case errc::success:
			return "Success";
		case errc::device_not_found:
			return "Device not found";
		case errc::device_not_available:
			return "Device not available";
		case errc::compiler_not_available:
			return "Compiler not available";
		case errc::mem_object_allocation_failure:
			return "Memory object allocation failure";
		case errc::out_of_resources:
			return "Out of resources";
		case errc::out_of_host_memory:
			return "Out of host memory";
		case errc::profiling_info_not_available:
			return "Profiling information not available";
		case errc::mem_copy_overlap:
			return "Memory copy overlap";
		case errc::image_format_mismatch:
			return "Image format mismatch";
		case errc::image_format_not_supported:
			return "Image format not supported";
		case errc::build_program_failure:
			return "Build program failure";
		case errc::map_failure:
			return "Map failure";
		#if CL_TARGET_VERSION >= 110
		case errc::misaligned_sub_buffer_offset:
			return "Misaligned sub buffer offset";
		case errc::exec_status_error_for_events_in_wait_list:
			return "Exec status error for events in wait list";
		#endif
		#if CL_TARGET_VERSION >= 120
		case errc::compile_program_failure:
			return "Compile program failure";
		case errc::linker_not_available:
			return "Linker not available";
		case errc::link_program_failure:
			return "Link program failure";
		case errc::device_partition_failed:
			return "Device partition failed";
		case errc::kernel_arg_info_not_available:
			return "Kernel argument information not available";
		#endif
		case errc::invalid_value:
			return "Invalid value";
		case errc::invalid_device_type:
			return "Invalid device type";
		case errc::invalid_platform:
			return "Invalid platform";
		case errc::invalid_device:
			return "Invalid device";
		case errc::invalid_context:
			return "Invalid context";
		case errc::invalid_queue_properties:
			return "Invalid queue properties";
		case errc::invalid_command_queue:
			return "Invalid command queue";
		case errc::invalid_host_ptr:
			return "Invalid host ptr";
		case errc::invalid_mem_object:
			return "Invalid memory object";
		case errc::invalid_image_format_descriptor:
			return "Invalid image format descriptor";
		case errc::invalid_image_size:
			return "Invalid image size";
		case errc::invalid_sampler:
			return "Invalid sampler";
		case errc::invalid_binary:
			return "Invalid binary";
		case errc::invalid_build_options:
			return "Invalid build options";
		case errc::invalid_program:
			return "Invalid program";
		case errc::invalid_program_executable:
			return "Invalid program executable";
		case errc::invalid_kernel_name:
			return "Invalid kernel name";
		case errc::invalid_kernel_definition:
			return "Invalid kernel definition";
		case errc::invalid_kernel:
			return "Invalid kernel";
		case errc::invalid_arg_index:
			return "Invalid argument index";
		case errc::invalid_arg_value:
			return "Invalid argument value";
		case errc::invalid_arg_size:
			return "Invalid argument size";
		case errc::invalid_kernel_args:
			return "Invalid kernel arguments";
		case errc::invalid_work_dimension:
			return "Invalid work dimension";
		case errc::invalid_work_group_size:
			return "Invalid work group size";
		case errc::invalid_work_item_size:
			return "Invalid work item size";
		case errc::invalid_global_offset:
			return "Invalid global offset";
		case errc::invalid_event_wait_list:
			return "Invalid event wait list";
		case errc::invalid_event:
			return "Invalid event";
		case errc::invalid_operation:
			return "Invalid operation";
		case errc::gl_invalid_object: return "Invalid object (GL)";
		case errc::invalid_buffer_size:
			return "Invalid buffer size";
		case errc::invalid_mip_level:
			return "Invalid mip level";
		case errc::invalid_global_work_size:
			return "Invalid global work size";
		#if CL_TARGET_VERSION >= 110
		case errc::invalid_property:
			return "Invalid property";
		#endif
		#if CL_TARGET_VERSION >= 120
		case errc::invalid_image_descriptor:
			return "Invalid image descriptor";
		case errc::invalid_compiler_options:
			return "Invalid compiler options";
		case errc::invalid_linker_options:
			return "Invalid linker options";
		case errc::invalid_device_partition_count:
			return "Invalid device partition count";
		#endif
		#if CL_TARGET_VERSION >= 200
		case errc::invalid_pipe_size:
			return "Invalid pipe size";
		case errc::invalid_device_queue:
			return "Invalid device queue";
		#endif
		#if CL_TARGET_VERSION >= 220
		case errc::invalid_spec_id:
			return "Invalid spec id";
		case errc::max_size_restriction_exceeded:
			return "Max size restriction exceeded";
		#endif
		case errc::illegal_read_or_write_to_a_buffer:
			return "Illegal read or write to a buffer";
		#if defined(CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR)
		case errc::gl_invalid_sharegroup_reference: return "Invalid share group reference (GL)";
		#endif
		#if defined(CL_PLATFORM_NOT_FOUND_KHR)
		case errc::platform_not_found: return "Platform not found";
		#endif
		#if defined(CL_DEVICE_PARTITION_FAILED_EXT)
		case errc::ext_device_partition_failed: return "Partition failed";
		#endif
		#if defined(CL_INVALID_PARTITION_COUNT_EXT)
		case errc::ext_invalid_partition_count: return "Invalid partition count";
		#endif
		#if defined(CL_INVALID_PARTITION_NAME_EXT)
		case errc::ext_invalid_partition_name: return "Invalid partition name";
		#endif
		#if defined(CL_GRALLOC_RESOURCE_NOT_ACQUIRED_IMG)
		case errc::gralloc_resource_not_acquired: return "Resource not acquired (Gralloc)";
		#endif
		#if defined(CL_INVALID_EGL_OBJECT_KHR)
		case errc::egl_invalid_object: return "Invalid object (EGL)";
		#endif
		#if defined(CL_EGL_RESOURCE_NOT_ACQUIRED_KHR)
		case egl_resource_not_acquired: return "Resource not acquired (EGL)";
		#endif
		default: return nullptr;
	}
}

std::ostream&
clx::operator<<(std::ostream& out, const errc& rhs) {
	if (const char* s = to_string(rhs)) {
		out << s;
	} else {
		out << static_cast<int_type>(rhs);
	}
	return out;
}


clx::error_category clx::opencl_category;

std::string
clx::error_category::message(int ev) const noexcept {
	auto cond = static_cast<errc>(ev);
	const char* str = to_string(cond);
	if (str) { return str; }
	std::stringstream msg;
	msg << static_cast<int_type>(cond);
	return msg.str();
}

