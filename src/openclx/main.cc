#include <openclx/compiler>
#include <openclx/platform>

#include <iostream>

int main() {
	using namespace clx;
	for (const auto& p : platforms()) {
		std::clog << "p.name()=" << p.name() << std::endl;
		for (const auto& d : p.devices(device_flags::all)) {
			std::clog << "d.name()=" << d.name() << std::endl;
			std::clog << "d.vendor_id()=" << d.vendor_id() << std::endl;
		}
	}
	return 0;
}
