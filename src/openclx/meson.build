openclx_src = files([
	'binary.cc',
	'buffer.cc',
	'command_queue.cc',
	'compiler.cc',
	'context.cc',
	'context_properties.cc',
	'device.cc',
	'error.cc',
	'event.cc',
	'event_stack.cc',
	'image.cc',
	'kernel_argument.cc',
	'kernel.cc',
	'memory_object.cc',
	'pipe.cc',
	'platform.cc',
	'program.cc',
	'sampler.cc',
	'svm_block.cc',
])

openclx_deps = [
	dependency('OpenCL')
]

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
	libraries: libopenclx,
	version: meson.project_version(),
	name: meson.project_name(),
	description: 'C++ wrapper for OpenCL',
	filebase: meson.project_name()
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
endif
