#include <openclx/array_view>
#include <openclx/buffer>
#include <openclx/downcast>
#include <openclx/error>
#include <openclx/event_stack>
#include <openclx/image>
#include <openclx/kernel>

#define CLX_BODY_ENQUEUE(name, ...) \
	auto& frame = this->frame(); \
	event_type ret; \
	{ \
		step_guard g(this->_sync, frame); \
		CLX_CHECK(name( \
			this->_queue.get(), __VA_ARGS__, g.nevents, g.events, &ret \
		)); \
	} \
	frame.emplace_back(std::move(ret));

#define CLX_BODY_ENQUEUE_NOARGS(name) \
	auto& frame = this->frame(); \
	event_type ret; \
	{ \
		step_guard g(this->_sync, frame); \
		CLX_CHECK(name( \
			this->_queue.get(), g.nevents, g.events, &ret \
		)); \
	} \
	frame.emplace_back(std::move(ret));

#define CLX_BODY_ENQUEUE_MAP(name, ...) \
	auto& frame = this->frame(); \
	event_type ret; \
	host_pointer ptr = nullptr; \
	{ \
		step_guard g(this->_sync, frame); \
		int_type err = 0; \
		ptr = name( \
			this->_queue.get(), __VA_ARGS__, \
			g.nevents, g.events, &ret, &err \
		); \
		CLX_CHECK(err); \
	} \
	frame.emplace_back(std::move(ret)); \
	return ptr;

void
clx::event_stack::wait() {
	auto& frame = this->frame();
	CLX_CHECK(::clWaitForEvents(frame.size(), downcast(frame.data())));
	frame.clear();
}

#if CL_TARGET_VERSION >= 120
void
clx::event_stack::barrier_120() {
	CLX_BODY_ENQUEUE_NOARGS(::clEnqueueBarrierWithWaitList);
}
#endif

#if CL_TARGET_VERSION <= 110 || defined(CL_USE_DEPRECATED_OPENCL_1_1_APIS)
void
clx::event_stack::barrier_100() {
	auto& frame = this->frame();
	event_type ret;
	CLX_CHECK(::clEnqueueBarrier(this->_queue.get(), &ret));
	frame.emplace_back(std::move(ret));
}
#endif

#if CL_TARGET_VERSION >= 120
void
clx::event_stack::marker_120() {
	CLX_BODY_ENQUEUE_NOARGS(::clEnqueueMarkerWithWaitList);
}
#endif

#if CL_TARGET_VERSION <= 110 || defined(CL_USE_DEPRECATED_OPENCL_1_1_APIS)
void
clx::event_stack::marker_100() {
	auto& frame = this->frame();
	event_type ret;
	CLX_CHECK(::clEnqueueMarker(this->_queue.get(), &ret));
	frame.emplace_back(std::move(ret));
}
#endif

void
clx::event_stack::kernel(const ::clx::kernel& k) {
	const size_t offset = 0, size = 1;
	CLX_BODY_ENQUEUE(
		::clEnqueueNDRangeKernel, k.get(),
		1u, &offset, &size, &size
	);
}

void
clx::event_stack::kernel(
	const ::clx::kernel& k,
	const range& offset,
	const range& global,
	const range& local
) {
	CLX_BODY_ENQUEUE(
		::clEnqueueNDRangeKernel, k.get(),
		global.dimensions(), offset.data(), global.data(), local.data()
	);
}

void
clx::event_stack::copy(const buffer_slice& src, host_pointer dst) {
	CLX_BODY_ENQUEUE(
		::clEnqueueReadBuffer,
		src.buffer.get(), CL_FALSE,
		src.offset, src.size, dst
	);
}

#if CL_TARGET_VERSION >= 110
void
clx::event_stack::copy(const buffer_slice_3d& src, const host_slice_3d& dst) {
	CLX_BODY_ENQUEUE(
		::clEnqueueReadBufferRect,
		src.object.get(), CL_FALSE,
		src.offset.data(), dst.offset.data(), src.size.data(),
		src.row_pitch, src.slice_pitch,
		dst.row_pitch, dst.slice_pitch,
		dst.object
	);
}
#endif

void
clx::event_stack::copy(const image_slice_3d& src, host_pointer dst) {
	CLX_BODY_ENQUEUE(
		::clEnqueueReadImage, src.object.get(), CL_FALSE,
		src.offset.data(), src.size.data(),
		src.row_pitch, src.slice_pitch, dst
	);
}

void
clx::event_stack::copy(const image& src, host_pointer dst) {
	this->copy({src,{0,0,0},src.dimensions()}, dst);
}

void
clx::event_stack::copy(const_host_pointer src, const buffer_slice& dst) {
	CLX_BODY_ENQUEUE(
		::clEnqueueWriteBuffer, dst.buffer.get(), CL_FALSE,
		dst.offset, dst.size, src
	);
}

#if CL_TARGET_VERSION >= 110
void
clx::event_stack::copy(
	const const_host_slice_3d& src,
	const buffer_slice_3d& dst
) {
	CLX_BODY_ENQUEUE(
		::clEnqueueWriteBufferRect, dst.object.get(), CL_FALSE,
		dst.offset.data(), src.offset.data(), dst.size.data(),
		dst.row_pitch, dst.slice_pitch, src.row_pitch, dst.slice_pitch,
		src.object
	);
}
#endif

void
clx::event_stack::copy(const_host_pointer src, const image_slice_3d& dst) {
	CLX_BODY_ENQUEUE(
		::clEnqueueWriteImage, dst.object.get(), CL_FALSE,
		dst.offset.data(), dst.size.data(),
		dst.row_pitch, dst.slice_pitch, src
	);
}

void
clx::event_stack::copy(const_host_pointer src, const image& dst) {
	this->copy(src, dst.slice({0,0,0},dst.dimensions()));
}

void
clx::event_stack::copy(const buffer_slice& src, const buffer_slice& dst) {
	CLX_BODY_ENQUEUE(
		::clEnqueueCopyBuffer, src.buffer.get(), dst.buffer.get(),
		src.offset, dst.offset, src.size
	);
}

void
clx::event_stack::copy(const buffer& src, const buffer& dst) {
	const auto size = std::min(src.size(), dst.size());
	this->copy(src.slice(0,size), dst.slice(0,size));
}

void
clx::event_stack::copy(const image_slice_3d& src, const image_slice_3d& dst) {
	CLX_BODY_ENQUEUE(
		::clEnqueueCopyImage, src.object.get(), dst.object.get(),
		src.offset.data(), dst.offset.data(), src.size.data()
	);
}

void
clx::event_stack::copy(const image& src, const image& dst) {
	const auto& size = src.dimensions();
	this->copy(src.slice({0,0,0},size), dst.slice({0,0,0},size));
}

void
clx::event_stack::copy(const image_slice_3d& src, const buffer_slice& dst) {
	CLX_BODY_ENQUEUE(
		::clEnqueueCopyImageToBuffer, src.object.get(), dst.buffer.get(),
		src.offset.data(), src.size.data(), dst.offset
	);
}

void
clx::event_stack::copy(const image_slice_3d& src, const buffer& dst) {
	this->copy(src, dst.slice(0,0));
}

void
clx::event_stack::copy(const image& src, const buffer& dst) {
	this->copy(src.slice({0,0,0},src.dimensions()), dst);
}

void
clx::event_stack::copy(const buffer_slice& src, const image_slice_3d& dst) {
	CLX_BODY_ENQUEUE(
		::clEnqueueCopyBufferToImage, src.buffer.get(), dst.object.get(),
		src.offset, dst.offset.data(), dst.size.data()
	);
}

void
clx::event_stack::copy(const buffer_slice& src, const image& dst) {
	this->copy(src, dst.slice({0,0,0},dst.dimensions()));
}

void
clx::event_stack::copy(const buffer& src, const image& dst) {
	this->copy(src.slice(0,0), dst.slice({0,0,0},dst.dimensions()));
}

void
clx::event_stack::copy(const buffer_slice_3d& src, const buffer_slice_3d& dst) {
	CLX_BODY_ENQUEUE(
		::clEnqueueCopyBufferRect, src.object.get(), dst.object.get(),
		src.offset.data(), dst.offset.data(), dst.size.data(),
		src.row_pitch, src.slice_pitch, dst.row_pitch, dst.slice_pitch
	);
}

#if CL_TARGET_VERSION >= 120
void
clx::event_stack::fill(const buffer_slice& dst, const pattern& pattern) {
	CLX_BODY_ENQUEUE(
		::clEnqueueFillBuffer, dst.buffer.get(),
		pattern.ptr(), pattern.size(),
		dst.offset, dst.size
	);
}
#endif

#if CL_TARGET_VERSION >= 120
void
clx::event_stack::fill(const buffer& dst, const pattern& pattern) {
	this->fill(dst.slice(0,dst.size()), pattern);
}
#endif

#if CL_TARGET_VERSION >= 120
void
clx::event_stack::fill(const image_slice_3d& dst, const color& col) {
	CLX_BODY_ENQUEUE(
		::clEnqueueFillImage, dst.object.get(),
		&col, dst.offset.data(), dst.size.data()
	);
}
#endif

#if CL_TARGET_VERSION >= 120
void
clx::event_stack::fill(const image& dst, const color& col) {
	this->fill(dst.slice({0,0,0},dst.dimensions()), col);
}
#endif

#if CL_TARGET_VERSION >= 120
void
clx::event_stack::migrate(
	migration_flags flags,
	const memory_object_array& objects
) {
	CLX_BODY_ENQUEUE(
		::clEnqueueMigrateMemObjects,
		objects.size(), downcast(objects.data()), downcast(flags)
	);
}
#endif

#if CL_TARGET_VERSION >= 200
void
clx::event_stack::free(const array_view<void*>& pointers) {
	CLX_BODY_ENQUEUE(
		::clEnqueueSVMFree,
		pointers.size(), const_cast<void**>(pointers.data()),
		nullptr, nullptr
	);
}

void
clx::event_stack::fill(svm_array ptr, const pattern& pattern) {
	CLX_BODY_ENQUEUE(
		::clEnqueueSVMMemFill, ptr.data(),
		pattern.ptr(), pattern.size(),
		ptr.size()
	);
}

void
clx::event_stack::copy(const svm_array& src, const svm_array& dst) {
	CLX_BODY_ENQUEUE(
		::clEnqueueSVMMemcpy, CL_FALSE,
		dst.data(), src.data(), src.size()
	);
}

void
clx::event_stack::map(const svm_array& src, map_flags flags) {
	CLX_BODY_ENQUEUE(
		::clEnqueueSVMMap, CL_FALSE,
		downcast(flags), src.data(), src.size()
	);
}

void
clx::event_stack::unmap(svm_pointer ptr) {
	CLX_BODY_ENQUEUE(::clEnqueueSVMUnmap, ptr);
}
#endif

#if CL_TARGET_VERSION >= 210
void
clx::event_stack::migrate(
	migration_flags flags,
	const array_view<array_view<svm_pointer>>& pointers
) {
	std::vector<const void*> ptrs;
	ptrs.reserve(pointers.size());
	std::vector<size_t> sizes;
	sizes.reserve(pointers.size());
	for (const auto& p : pointers) {
		ptrs.emplace_back(p.data());
		sizes.emplace_back(p.size());
	}
	CLX_BODY_ENQUEUE(
		::clEnqueueSVMMigrateMem,
		pointers.size(), ptrs.data(), sizes.data(), downcast(flags)
	);
}
#endif

clx::host_pointer
clx::event_stack::map(const buffer_slice& b, map_flags flags) {
	CLX_BODY_ENQUEUE_MAP(
		::clEnqueueMapBuffer, b.buffer.get(), CL_FALSE,
		downcast(flags), b.offset, b.size
	);
}

clx::host_pointer
clx::event_stack::map(const buffer& b, map_flags flags) {
	return this->map(b.slice(0,b.size()), flags);
}

clx::host_pointer
clx::event_stack::map(
	const image_slice_3d& src, map_flags flags,
	size_t& row_pitch, size_t& slice_pitch
) {
	CLX_BODY_ENQUEUE_MAP(
		::clEnqueueMapImage,
		src.object.get(), CL_FALSE, downcast(flags),
		src.offset.data(), src.size.data(), &row_pitch, &slice_pitch
	);
}

clx::host_pointer
clx::event_stack::map(
	const image& src, map_flags flags,
	size_t& row_pitch, size_t& slice_pitch
) {
	return this->map(
		src.slice({0,0,0},src.dimensions()),
		flags, row_pitch, slice_pitch
	);
}

void
clx::event_stack::unmap(const memory_object& obj, host_pointer ptr) {
	CLX_BODY_ENQUEUE(::clEnqueueUnmapMemObject, obj.get(), ptr);
}

