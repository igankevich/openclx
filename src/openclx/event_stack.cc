#include <openclx/buffer>
#include <openclx/error>
#include <openclx/event_stack>
#include <openclx/image>
#include <openclx/kernel>

namespace {

	inline void
	release(clx::event_type* data, size_t n) {
		for (size_t i=0; i<n; ++i) {
			CLX_CHECK(::clReleaseEvent(data[i]));
		}
	}

	inline const clx::event_type*
	downcast(const clx::event* x) {
		return reinterpret_cast<const clx::event_type*>(x);
	}

	inline clx::event_type*
	downcast(clx::event* x) {
		return reinterpret_cast<clx::event_type*>(x);
	}

}

void
clx::event_stack::wait() {
	auto& frame = this->frame();
	CLX_CHECK(::clWaitForEvents(frame.size(), downcast(frame.data())));
	frame.clear();
}

void
clx::event_stack::barrier() {
	auto& frame = this->frame();
	event_type ret;
	{
		#if CL_TARGET_VERSION <= 110 || defined(CL_USE_DEPRECATED_OPENCL_1_1_APIS)
		CLX_CHECK(::clEnqueueBarrier(this->_queue.get(), &ret));
		#else
		step_guard g(this->_sync, frame);
		CLX_CHECK(::clEnqueueBarrierWithWaitList(
			this->_queue.get(),
			g.nevents,
			g.events,
			&ret
		));
		#endif
	}
	frame.emplace_back(std::move(ret));
}

void
clx::event_stack::marker() {
	auto& frame = this->frame();
	event_type ret;
	{
		#if CL_TARGET_VERSION <= 110 || defined(CL_USE_DEPRECATED_OPENCL_1_1_APIS)
		CLX_CHECK(::clEnqueueMarker(this->_queue.get(), &ret));
		#else
		step_guard g(this->_sync, frame);
		CLX_CHECK(::clEnqueueMarkerWithWaitList(
			this->_queue.get(),
			g.nevents,
			g.events,
			&ret
		));
		#endif
	}
	frame.emplace_back(std::move(ret));
}

void
clx::event_stack::kernel(const ::clx::kernel& k) {
	auto& frame = this->frame();
	event_type ret;
	{
		step_guard g(this->_sync, frame);
		#if CL_TARGET_VERSION <= 120 || defined(CL_USE_DEPRECATED_OPENCL_1_2_APIS)
		CLX_CHECK(::clEnqueueTask(
			this->_queue.get(), k.get(),
			g.nevents, g.events, &ret
		));
		#else
		const size_t offset = 0, size = 1;
		CLX_CHECK(::clEnqueueNDRangeKernel(
			this->_queue.get(), k.get(),
			1u, &offset, &size, &size,
			g.nevents, g.events, &ret
		));
		#endif
	}
	frame.emplace_back(std::move(ret));
}

void
clx::event_stack::kernel(
	const ::clx::kernel& k,
	const range& offset,
	const range& global,
	const range& local
) {
	auto& frame = this->frame();
	event_type ret;
	{
		step_guard g(this->_sync, frame);
		CLX_CHECK(::clEnqueueNDRangeKernel(
			this->_queue.get(), k.get(), global.dimensions(),
			offset.data(), global.data(), local.data(),
			g.nevents, g.events, &ret
		));
	}
	frame.emplace_back(std::move(ret));
}

void
clx::event_stack::read(const buffer_slice& src, void* dst) {
	auto& frame = this->frame();
	event_type ret;
	{
		step_guard g(this->_sync, frame);
		CLX_CHECK(::clEnqueueReadBuffer(
			this->_queue.get(), src.buffer.get(), CL_FALSE,
			src.offset, src.size, dst,
			g.nevents, g.events, &ret
		));
	}
	frame.emplace_back(std::move(ret));
}

void
clx::event_stack::read(const image_slice& src, void* dst) {
	auto& frame = this->frame();
	event_type ret;
	{
		step_guard g(this->_sync, frame);
		CLX_CHECK(::clEnqueueReadImage(
			this->_queue.get(), src.image.get(), CL_FALSE,
			src.offset.data(), src.size.data(),
			src.image.row_pitch(), src.image.slice_pitch(), dst,
			g.nevents, g.events, &ret
		));
	}
	frame.emplace_back(std::move(ret));
}

void
clx::event_stack::read(const image& src, void* dst) {
	this->read({src,{0,0,0},src.dimensions()}, dst);
}

void
clx::event_stack::write(const buffer_slice& dst, void* src) {
	auto& frame = this->frame();
	event_type ret;
	{
		step_guard g(this->_sync, frame);
		CLX_CHECK(::clEnqueueWriteBuffer(
			this->_queue.get(), dst.buffer.get(), CL_FALSE,
			dst.offset, dst.size, src,
			g.nevents, g.events, &ret
		));
	}
	frame.emplace_back(std::move(ret));
}

void
clx::event_stack::write(const image_slice& dst, void* src) {
	auto& frame = this->frame();
	event_type ret;
	{
		step_guard g(this->_sync, frame);
		CLX_CHECK(::clEnqueueWriteImage(
			this->_queue.get(), dst.image.get(), CL_FALSE,
			dst.offset.data(), dst.size.data(),
			dst.image.row_pitch(), dst.image.slice_pitch(), src,
			g.nevents, g.events, &ret
		));
	}
	frame.emplace_back(std::move(ret));
}

void
clx::event_stack::write(const image& dst, void* src) {
	this->write(dst.slice({0,0,0},dst.dimensions()), src);
}

void
clx::event_stack::copy(const buffer_slice& src, const buffer_slice& dst) {
	auto& frame = this->frame();
	event_type ret;
	{
		step_guard g(this->_sync, frame);
		CLX_CHECK(::clEnqueueCopyBuffer(
			this->_queue.get(), src.buffer.get(), dst.buffer.get(),
			src.offset, dst.offset, src.size,
			g.nevents, g.events, &ret
		));
	}
	frame.emplace_back(std::move(ret));
}

void
clx::event_stack::copy(const buffer& src, const buffer& dst) {
	const auto size = std::min(src.size(), dst.size());
	this->copy(src.slice(0,size), dst.slice(0,size));
}

void
clx::event_stack::copy(const image_slice& src, const image_slice& dst) {
	auto& frame = this->frame();
	event_type ret;
	{
		step_guard g(this->_sync, frame);
		CLX_CHECK(::clEnqueueCopyImage(
			this->_queue.get(), src.image.get(), dst.image.get(),
			src.offset.data(), dst.offset.data(), src.size.data(),
			g.nevents, g.events, &ret
		));
	}
	frame.emplace_back(std::move(ret));
}

void
clx::event_stack::copy(const image& src, const image& dst) {
	const auto& size = src.dimensions();
	this->copy(src.slice({0,0,0},size), dst.slice({0,0,0},size));
}

void
clx::event_stack::copy(const image_slice& src, const buffer_slice& dst) {
	auto& frame = this->frame();
	event_type ret;
	{
		step_guard g(this->_sync, frame);
		CLX_CHECK(::clEnqueueCopyImageToBuffer(
			this->_queue.get(), src.image.get(), dst.buffer.get(),
			src.offset.data(), src.size.data(), dst.offset,
			g.nevents, g.events, &ret
		));
	}
	frame.emplace_back(std::move(ret));
}

void
clx::event_stack::copy(const image_slice& src, const buffer& dst) {
	this->copy(src, dst.slice(0,0));
}

void
clx::event_stack::copy(const image& src, const buffer& dst) {
	this->copy(src.slice({0,0,0},src.dimensions()), dst);
}

void
clx::event_stack::copy(const buffer_slice& src, const image_slice& dst) {
	auto& frame = this->frame();
	event_type ret;
	{
		step_guard g(this->_sync, frame);
		CLX_CHECK(::clEnqueueCopyBufferToImage(
			this->_queue.get(), src.buffer.get(), dst.image.get(),
			src.offset, dst.offset.data(), dst.size.data(),
			g.nevents, g.events, &ret
		));
	}
	frame.emplace_back(std::move(ret));
}

void
clx::event_stack::copy(const buffer_slice& src, const image& dst) {
	this->copy(src, dst.slice({0,0,0},dst.dimensions()));
}

void
clx::event_stack::copy(const buffer& src, const image& dst) {
	this->copy(src.slice(0,0), dst.slice({0,0,0},dst.dimensions()));
}

void*
clx::event_stack::map(const buffer_slice& b, map_flags flags) {
	auto& frame = this->frame();
	event_type ret;
	void* ptr = nullptr;
	{
		step_guard g(this->_sync, frame);
		int_type err = 0;
		ptr = ::clEnqueueMapBuffer(
			this->_queue.get(), b.buffer.get(), CL_FALSE,
			static_cast<map_flags_type>(flags), b.offset, b.size,
			g.nevents, g.events, &ret, &err
		);
		CLX_CHECK(err);
	}
	frame.emplace_back(std::move(ret));
	return ptr;
}

void*
clx::event_stack::map(const buffer& b, map_flags flags) {
	return this->map(b.slice(0,b.size()), flags);
}

void*
clx::event_stack::map(
	const image_slice& src, map_flags flags,
	size_t& row_pitch, size_t& slice_pitch
) {
	auto& frame = this->frame();
	event_type ret;
	void* ptr = nullptr;
	{
		step_guard g(this->_sync, frame);
		int_type err = 0;
		ptr = ::clEnqueueMapImage(
			this->_queue.get(), src.image.get(), CL_FALSE,
			static_cast<map_flags_type>(flags), src.offset.data(), src.size.data(),
			&row_pitch, &slice_pitch,
			g.nevents, g.events, &ret, &err
		);
		CLX_CHECK(err);
	}
	frame.emplace_back(std::move(ret));
	return ptr;
}

void*
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
clx::event_stack::unmap(const memory_object& obj, void* ptr) {
	auto& frame = this->frame();
	event_type ret;
	{
		step_guard g(this->_sync, frame);
		CLX_CHECK(::clEnqueueUnmapMemObject(
			this->_queue.get(), obj.get(), ptr,
			g.nevents, g.events, &ret
		));
	}
	frame.emplace_back(std::move(ret));
}

