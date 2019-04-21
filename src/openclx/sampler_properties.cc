#include <openclx/extensions>
#include <openclx/platform>
#include <openclx/sampler_properties>

#if CL_TARGET_VERSION >= 200
std::vector<clx::sampler_properties_type>
clx::sampler_properties::operator()(const extensions& extensions) const {
	std::vector<sampler_properties_type> prop{
		sampler_properties_type(CL_SAMPLER_NORMALIZED_COORDS),
		sampler_properties_type(normalised()),
		sampler_properties_type(CL_SAMPLER_ADDRESSING_MODE),
		sampler_properties_type(get_addressing_mode()),
		sampler_properties_type(CL_SAMPLER_FILTER_MODE),
		sampler_properties_type(get_filter_mode())
	};
	if (extensions("cl_khr_mipmap_image")) {
		prop.push_back(sampler_properties_type(CL_SAMPLER_MIP_FILTER_MODE));
		prop.push_back(sampler_properties_type(mipmode()));
		prop.push_back(sampler_properties_type(CL_SAMPLER_LOD_MIN));
		prop.push_back(sampler_properties_type(lod_min()));
		prop.push_back(sampler_properties_type(CL_SAMPLER_LOD_MAX));
		prop.push_back(sampler_properties_type(lod_max()));
	}
	prop.emplace_back(0);
	return prop;
}
#endif
