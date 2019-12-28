#include <openclx/intel/platform>
#include <openclx/intel/device>

#if defined(cl_intel_dx9_media_sharing)
std::vector<clx::intel::device>
clx::intel::platform::devices(device_source src, void* object, device_set set) const {
    auto func = CLX_EXTENSION(clGetDeviceIDsFromDX9INTEL, *this);
    std::vector<device> result(4096 / sizeof(device));
    unsigned_int_type actual_size = 0;
    int_type ret;
    bool success = false;
    while (!success) {
        ret = func(
            this->_ptr, src, object, set,
            result.size(), downcast(result.data()), &actual_size
        );
        result.resize(actual_size);
        if (errc(ret) != errc::invalid_value && actual_size <= result.size()) {
            CLX_CHECK(ret);
            success = true;
        }
    }
    return result;
}
#endif

#if defined(cl_intel_va_api_media_sharing)
std::vector<clx::intel::device>
clx::intel::platform::devices(
    va::device_source src,
    void* object,
    va::device_set set
) const {
    auto func = CLX_EXTENSION(clGetDeviceIDsFromVA_APIMediaAdapterINTEL, *this);
    std::vector<device> result(4096 / sizeof(device));
    unsigned_int_type actual_size = 0;
    int_type ret;
    bool success = false;
    while (!success) {
        ret = func(
            this->_ptr, downcast(src), object, downcast(set),
            result.size(), downcast(result.data()), &actual_size
        );
        result.resize(actual_size);
        if (errc(ret) != errc::invalid_value && actual_size <= result.size()) {
            CLX_CHECK(ret);
            success = true;
        }
    }
    return result;
}
#endif

