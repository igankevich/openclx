#include <openclx/bits/macros>
#include <openclx/ext/device>

CLX_METHOD_SCALAR(
	clx::ext::device::parent,
	::clGetDeviceInfo,
	device,
	CL_DEVICE_PARENT_DEVICE_EXT
);

CLX_METHOD_SCALAR(
	clx::ext::device::num_references,
	::clGetDeviceInfo,
	unsigned_int_type,
	CL_DEVICE_REFERENCE_COUNT_EXT
);

CLX_METHOD_ARRAY(
	clx::ext::device::supported_partitions_priv,
	::clGetDeviceInfo,
	CL_DEVICE_PARTITION_TYPES_EXT,
	device_partition
)

CLX_METHOD_ARRAY(
	clx::ext::device::supported_affinity_domains_priv,
	::clGetDeviceInfo,
	CL_DEVICE_AFFINITY_DOMAINS_EXT,
	::clx::ext::affinity_domain
)

CLX_METHOD_ARRAY(
	clx::ext::device::partitions,
	::clGetDeviceInfo,
	CL_DEVICE_PARTITION_STYLE_EXT,
	partition_property_type
)

std::vector<clx::ext::device_partition>
clx::ext::device::supported_partitions() const {
	auto partitions = this->supported_partitions_priv();
	if (!partitions.empty() && partitions.front() == device_partition{}) {
		partitions.clear();
	}
	return partitions;
}

std::vector<clx::ext::affinity_domain>
clx::ext::device::supported_affinity_domains() const {
	auto domains = this->supported_affinity_domains_priv();
	if (!domains.empty() && domains.front() == ::clx::ext::affinity_domain{}) {
		domains.clear();
	}
	return domains;
}

void
clx::ext::device::release() {
	if (this->_ptr) {
		auto func = CLX_EXTENSION(clReleaseDeviceEXT, platform());
		CLX_CHECK(func(this->_ptr));
		this->_ptr = nullptr;
	}
}

void
clx::ext::device::retain() {
	if (this->_ptr) {
		auto func = CLX_EXTENSION(clRetainDeviceEXT, platform());
		CLX_CHECK(func(this->_ptr));
	}
}

