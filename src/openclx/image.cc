#include <openclx/bits/macros>
#include <openclx/image>

CLX_METHOD_SCALAR(
	clx::image::format,
	::clGetImageInfo,
	image_format,
	CL_IMAGE_FORMAT
)

CLX_METHOD_SCALAR(
	clx::image::element_size,
	::clGetImageInfo,
	size_t,
	CL_IMAGE_ELEMENT_SIZE
)

CLX_METHOD_SCALAR(
	clx::image::row_pitch,
	::clGetImageInfo,
	size_t,
	CL_IMAGE_ROW_PITCH
)

CLX_METHOD_SCALAR(
	clx::image::slice_pitch,
	::clGetImageInfo,
	size_t,
	CL_IMAGE_SLICE_PITCH
)

CLX_METHOD_SCALAR(clx::image::width, ::clGetImageInfo, size_t, CL_IMAGE_WIDTH)
CLX_METHOD_SCALAR(clx::image::height, ::clGetImageInfo, size_t, CL_IMAGE_HEIGHT)
CLX_METHOD_SCALAR(clx::image::depth, ::clGetImageInfo, size_t, CL_IMAGE_DEPTH)
