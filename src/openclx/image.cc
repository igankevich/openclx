#include <openclx/bits/macros>
#include <openclx/image>

CLX_GET_STRUCT4(
	clx::image::format,
	::clGetImageInfo,
	CL_IMAGE_FORMAT,
	image_format
)

CLX_GET_SCALAR5(
	clx::image::element_size,
	::clGetImageInfo,
	CL_IMAGE_ELEMENT_SIZE,
	size_t,
	size_t
)

CLX_GET_SCALAR5(
	clx::image::row_pitch,
	::clGetImageInfo,
	CL_IMAGE_ROW_PITCH,
	size_t,
	size_t
)

CLX_GET_SCALAR5(
	clx::image::slice_pitch,
	::clGetImageInfo,
	CL_IMAGE_SLICE_PITCH,
	size_t,
	size_t
)

CLX_GET_SCALAR5(
	clx::image::width,
	::clGetImageInfo,
	CL_IMAGE_WIDTH,
	size_t,
	size_t
)

CLX_GET_SCALAR5(
	clx::image::height,
	::clGetImageInfo,
	CL_IMAGE_HEIGHT,
	size_t,
	size_t
)

CLX_GET_SCALAR5(
	clx::image::depth,
	::clGetImageInfo,
	CL_IMAGE_DEPTH,
	size_t,
	size_t
)
