configure_file(
    input: 'config.in',
    output: 'config',
    configuration: config,
    install_dir: join_paths(
        get_option('prefix'),
        get_option('includedir'),
        'openclx'
    )
)

openclx_src = files([
    'binary.cc',
    'buffer.cc',
    'command_queue.cc',
    'command_queue_flags.cc',
    'compiler.cc',
    'context.cc',
    'context_properties.cc',
    'device.cc',
    'error.cc',
    'event.cc',
    'command_stack.cc',
    'image.cc',
    'kernel.cc',
    'kernel_argument.cc',
    'memory_object.cc',
    'pipe.cc',
    'platform.cc',
    'program.cc',
    'sampler.cc',
    'sampler_properties.cc',
    'svm_block.cc',
])

install_headers(
    'array_view',
    'binary',
    'binary_types',
    'buffer',
    'build_status',
    'cl',
    'color',
    'command',
    'command_queue',
    'command_queue_flags',
    'command_stack',
    'compiler',
    'context',
    'context_properties',
    'device',
    'device_affinity_domain',
    'device_flags',
    'device_local_memory',
    'device_memory_cache',
    'device_partition',
    'downcast',
    'error',
    'event',
    'execution_flags',
    'extensions',
    'floating_point_flags',
    'forward',
    'image',
    'image_descriptor',
    'image_format',
    'kernel',
    'kernel_argument',
    'limits',
    'map_flags',
    'memory_flags',
    'memory_object',
    'memory_objects',
    'migration_flags',
    'native_kernel',
    'pattern',
    'pipe',
    'platform',
    'program',
    'range',
    'sampler',
    'sampler_properties',
    'slice',
    'svm_allocator',
    'svm_block',
    'svm_flags',
    'vector',
    subdir: join_paths('openclx')
)

subdir('arm')
subdir('bits')
subdir('d3d10')
subdir('d3d11')
subdir('d3d9')
subdir('egl')
subdir('ext')
subdir('gl')
subdir('intel')
subdir('nv')
subdir('qcom')

opencl = dependency('OpenCL')
openclx_deps = [opencl]

libopenclx = library(
    'openclx',
    sources: openclx_src,
    include_directories: src,
    dependencies: openclx_deps,
    implicit_include_directories: false,
    install: true,
    version: meson.project_version(),
)

pkgconfig = import('pkgconfig')
pkgconfig.generate(
    libopenclx,
    libraries: libopenclx,
    requires: openclx_deps,
    version: meson.project_version(),
    name: meson.project_name(),
    filebase: meson.project_name(),
    description: 'C++ wrapper for OpenCL',
)

executable(
    'clx',
    sources: 'main.cc',
    include_directories: src,
    dependencies: openclx_deps,
    link_with: libopenclx,
    implicit_include_directories: false,
    install: true,
)

if with_tests
    gtest = dependency('gtest', main: true)
    test(
        'device-test',
        executable(
            'device-test',
            sources: 'device_test.cc',
            include_directories: src,
            dependencies: [openclx_deps,gtest],
            link_with: libopenclx,
            implicit_include_directories: false,
        )
    )
    test(
        'extensions-text',
        executable(
            'extensions-text',
            sources: 'extensions_test.cc',
            include_directories: src,
            dependencies: [openclx_deps,gtest],
            link_with: libopenclx,
            implicit_include_directories: false,
        )
    )
endif
