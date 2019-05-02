#include <openclx/downcast>
#include <openclx/intel/device>

#if CL_TARGET_VERSION >= 120 && defined(cl_intel_device_partition_by_names)
std::vector<clx::intel::device>
clx::intel::device::partition_by_names(const std::vector<unsigned int>& names) const {
	std::vector<partition_property_type> prop;
	prop.reserve(3 + names.size());
	prop.emplace_back(CL_DEVICE_PARTITION_BY_NAMES_INTEL);
	for (auto n : names) {
		prop.emplace_back(partition_property_type(n));
	}
	prop.emplace_back(CL_PARTITION_BY_NAMES_LIST_END_INTEL);
	prop.emplace_back(CL_PROPERTIES_LIST_END_EXT);
	std::vector<device> result(names.size());
	unsigned_int_type actual_size = 0;
	CLX_CHECK(::clCreateSubDevices(
		this->_ptr, prop.data(),
		result.size(), downcast(result.data()),
		&actual_size
	));
	result.resize(actual_size);
	return result;
}
#endif

