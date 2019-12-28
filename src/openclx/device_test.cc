#include <iostream>

#include <gtest/gtest.h>

#include <openclx/compiler>
#include <openclx/platform>

#if CL_TARGET_VERSION >= 120
TEST(device, partition) {
    for (const auto& platform : clx::platforms()) {
        clx::device root;
        try {
            root = platform.devices(clx::device_flags::all).front();
        } catch (const std::exception& err) {
            std::clog << "skipping " << platform.name()
                << ": " << err.what() << std::endl;
            continue;
        }
        if (root.supported_partitions().empty()) {
            std::clog << "skipping " << platform.name()
                << ": partitions are not supported" << std::endl;
            continue;
        }
        auto num_units = root.max_compute_units();
        auto max_subdevices = root.max_subordinate_devices();
        auto num_units_per_device = num_units / max_subdevices;
        auto subdevices = root.partition(num_units_per_device);
        EXPECT_EQ(max_subdevices, subdevices.size());
    }
}
#endif
