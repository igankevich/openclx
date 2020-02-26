#include <gtest/gtest.h>

#include <openclx/command_stack>
#include <openclx/compiler>
#include <openclx/context>
#include <openclx/device>
#include <openclx/kernel>
#include <openclx/platform>
#include <openclx/program>

TEST(kernel, local) {
    for (const auto& platform : clx::platforms()) {
        clx::context ctx(platform);
        clx::compiler cc(ctx);
        auto devices = platform.devices(clx::device_flags::all);
        cc.devices(devices);
        clx::program prg = cc.compile("test", "kernel void test(local float* x) {}");
        clx::kernel kernel = prg.kernel("test");
        kernel.argument(0, clx::local<float>(16));
        clx::command_stack stack(
            clx::command_queue(ctx, devices.front(), clx::command_queue_flags{}),
            clx::command_queue(ctx, devices.front(), clx::command_queue_flags{})
        );
        stack.kernel(kernel);
        stack.wait();
        kernel = prg.kernel("test");
        kernel.arguments(clx::local<float>(16));
        stack.kernel(kernel);
        stack.wait();
    }
}
