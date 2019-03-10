#include <openclx/device>
#include <openclx/error>
#include <openclx/event>
#include <openclx/image_format>
#include <openclx/limits>
#include <openclx/platform>
#include <openclx/types>
#include <openclx/vector>

#include <iostream>

int main() {
	using namespace clx;
	for (const auto& p : platforms()) {
		std::clog << "p.name()=" << p.name() << std::endl;
		for (const auto& d : p.devices(CL_DEVICE_TYPE_ALL)) {
			std::clog << "d.name()=" << d.name() << std::endl;
		}
	}
	return 0;
}
