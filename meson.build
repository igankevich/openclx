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

# cppcheck {{{
cppcheck = find_program('cppcheck', required: false)
if cppcheck.found()
	run_target(
		'cppcheck',
		command: [
			cppcheck,
			'--enable=all',
			'--quiet',
			'--force',
			'--language=c++',
			'--std=c++11',
			'--template=gcc',
			'-I' + join_paths(meson.source_root(), 'src'),
			'-I' + join_paths(meson.build_root(), 'src'),
			join_paths(meson.source_root(), 'src'),
		]
	)
endif
# }}}
# clang-check {{{
clang_check = find_program('clang-check', required: false)
if clang_check.found()
	run_target(
		'clang-check',
		command: [clang_check, '-analyze'] + openclx_src + [
			'--',
			'-I' + join_paths(meson.source_root(), 'src'),
			'-I' + join_paths(meson.build_root(), 'src'),
			'-std=c++11'
		]
	)
endif
# }}}
# clang-tidy {{{
clang_tidy = find_program('clang-tidy', required: false)
if clang_tidy.found()
	run_target(
		'clang-tidy',
		command: [clang_check] + openclx_src + [
			'--',
			'-I' + join_paths(meson.source_root(), 'src'),
			'-I' + join_paths(meson.build_root(), 'src'),
			'-std=c++11'
		]
	)
endif
# }}}
