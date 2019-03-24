openclx_src = files([
	'binary.cc',
	'buffer.cc',
	'command_queue.cc',
	'context.cc',
	'device.cc',
	'error.cc',
	'event.cc',
	'event_stack.cc',
	'image.cc',
	'kernel.cc',
	'kernel_argument.cc',
	'main.cc',
	'memory_object.cc',
	'pipe.cc',
	'platform.cc',
	'program.cc',
	'sampler.cc',
])

openclx_deps = [
	dependency('OpenCL')
]

shared_library(
	'openclx',
	sources: openclx_src,
	include_directories: src,
	dependencies: openclx_deps,
	implicit_include_directories: false,
)

executable(
	'openclx',
	sources: openclx_src,
	include_directories: src,
	dependencies: openclx_deps,
	implicit_include_directories: false,
)
