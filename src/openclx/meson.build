openclx_src = files([
	'binary.cc',
	'buffer.cc',
	'command_queue.cc',
	'compiler.cc',
	'context.cc',
	'device.cc',
	'error.cc',
	'event.cc',
	'event_stack.cc',
	'image.cc',
	'kernel.cc',
	'kernel_argument.cc',
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

libclx = shared_library(
	'openclx',
	sources: openclx_src,
	include_directories: src,
	dependencies: openclx_deps,
	implicit_include_directories: false,
)

executable(
	'clx',
	sources: 'main.cc',
	include_directories: src,
	dependencies: openclx_deps,
	link_with: libclx,
	implicit_include_directories: false,
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
			link_with: libclx,
			implicit_include_directories: false,
		)
	)
endif
