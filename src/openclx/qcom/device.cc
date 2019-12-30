#include <openclx/bits/macros>
#include <openclx/qcom/device>

#if CL_TARGET_VERSION >= 110 && defined(cl_qcom_ext_host_ptr)
CLX_METHOD_SCALAR(
    clx::qcom::device::memory_padding,
    ::clGetDeviceInfo,
    size_t,
    CL_DEVICE_EXT_MEM_PADDING_IN_BYTES_QCOM
);
CLX_METHOD_SCALAR(
    clx::qcom::device::page_size,
    ::clGetDeviceInfo,
    size_t,
    CL_DEVICE_PAGE_SIZE_QCOM
);
::clx::qcom::image_pitch
clx::qcom::device::image_pitch(size_t width, size_t height, const image_format& format) const {
    auto func = CLX_EXTENSION(clGetDeviceImageInfoQCOM, platform());
    ::clx::qcom::image_pitch pitch{};
    CLX_CHECK(func(
        this->_ptr, width, height, &format,
        CL_IMAGE_ROW_PITCH, sizeof(unsigned_int_type), &pitch.row_pitch, nullptr
    ));
    CLX_CHECK(func(
        this->_ptr, width, height, &format,
        CL_IMAGE_ROW_ALIGNMENT_QCOM, sizeof(unsigned_int_type), &pitch.row_alignment, nullptr
    ));
    CLX_CHECK(func(
        this->_ptr, width, height, &format,
        CL_IMAGE_SLICE_PITCH, sizeof(unsigned_int_type), &pitch.slice_pitch, nullptr
    ));
    CLX_CHECK(func(
        this->_ptr, width, height, &format,
        CL_IMAGE_SLICE_ALIGNMENT_QCOM, sizeof(unsigned_int_type), &pitch.slice_alignment,
        nullptr
    ));
    return pitch;
}
#endif

