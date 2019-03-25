project(
	'openclx',
	'cpp',
	version: '0.1.0',
	meson_version: '>=0.46',
	default_options: ['cpp_std=c++11']
)

with_tests = get_option('with_tests')

cpp = meson.get_compiler('cpp')

foreach arg : ['-Werror=return-type']
	if cpp.has_argument(arg)
		add_global_arguments(arg, language: 'cpp')
	endif
endforeach

src = include_directories('src')

subdir('src')
