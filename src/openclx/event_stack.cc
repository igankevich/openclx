#include <openclx/error>
#include <openclx/event_stack>

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

	struct step_guard {

		bool _sync;
		std::vector<clx::event>& frame;
		clx::unsigned_int_type nevents = 0;
		const clx::event_type* events = nullptr;

		step_guard(bool sync, std::vector<clx::event>& frame):
		_sync(sync), frame(frame) {
			if (this->_sync && !frame.empty()) {
				events = downcast(frame.data());
				nevents = frame.size();
			}
		}

		~step_guard() {
			if (this->_sync) {
				frame.clear();
				this->_sync = false;
			}
		}

	};

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
		step_guard g(this->_sync, frame);
		CLX_CHECK(::clEnqueueBarrierWithWaitList(
			this->_queue.get(),
			g.nevents,
			g.events,
			&ret
		));
	}
	frame.emplace_back(std::move(ret));
}

void
clx::event_stack::marker() {
	auto& frame = this->frame();
	event_type ret;
	{
		step_guard g(this->_sync, frame);
		CLX_CHECK(::clEnqueueMarkerWithWaitList(
			this->_queue.get(),
			g.nevents,
			g.events,
			&ret
		));
	}
	frame.emplace_back(std::move(ret));
}

