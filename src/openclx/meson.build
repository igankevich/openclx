openclx_src = files([
	'binary.cc',
	'command_queue.cc',
	'context.cc',
	'device.cc',
	'error.cc',
	'event.cc',
	'kernel.cc',
	'main.cc',
	'platform.cc',
	'program.cc',
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
