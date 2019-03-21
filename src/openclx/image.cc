#include <openclx/bits/macros>
#include <openclx/image>

CLX_METHOD_SCALAR(
	clx::image::format,
	::clGetImageInfo,
	CL_IMAGE_FORMAT,
	image_format
)

CLX_METHOD_SCALAR(
	clx::image::element_size,
	::clGetImageInfo,
	CL_IMAGE_ELEMENT_SIZE,
	size_t
)

CLX_METHOD_SCALAR(
	clx::image::row_pitch,
	::clGetImageInfo,
	CL_IMAGE_ROW_PITCH,
	size_t
)

CLX_METHOD_SCALAR(
	clx::image::slice_pitch,
	::clGetImageInfo,
	CL_IMAGE_SLICE_PITCH,
	size_t
)

CLX_METHOD_SCALAR(
	clx::image::width,
	::clGetImageInfo,
	CL_IMAGE_WIDTH,
	size_t
)

CLX_METHOD_SCALAR(
	clx::image::height,
	::clGetImageInfo,
	CL_IMAGE_HEIGHT,
	size_t
)

CLX_METHOD_SCALAR(
	clx::image::depth,
	::clGetImageInfo,
	CL_IMAGE_DEPTH,
	size_t
)
