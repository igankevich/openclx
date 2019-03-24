#include <fstream>
#include <iomanip>
#include <iostream>
#include <locale>
#include <sstream>
#include <string>

#include <openclx/compiler>
#include <openclx/platform>

template <class Ch, class Tr>
inline void
trim_right(std::basic_string<Ch,Tr>& rhs, const std::locale& loc) {
	while (!rhs.empty() && isspace(rhs.back(), loc)) {
		rhs.pop_back();
	}
}

template <class Ch, class Tr>
inline void
trim_left(std::basic_string<Ch,Tr>& rhs, const std::locale& loc) {
	size_t i = 0;
	size_t n = rhs.size();
	while (i < n && isspace(rhs[i], loc)) {
		++i;
	}
	if (i > 0) {
		rhs = rhs.substr(i);
	}
}

template <class Ch, class Tr>
inline void
trim(std::basic_string<Ch,Tr>& rhs, const std::locale& loc) {
	trim_right(rhs, loc);
	trim_left(rhs, loc);
}

inline std::string
catenate(const std::vector<std::string>& files) {
	std::stringstream content;
	for (const auto& file : files) {
		std::ifstream in;
		in.exceptions(std::ios::failbit | std::ios::badbit);
		in.open(file, std::ios::in);
		content << in.rdbuf();
	}
	return content.str();
}

void
print_platforms() {
	for (const auto& platform : clx::platforms()) {
		std::cout
			<< std::setw(20) << platform.suffix()
			<< ' ' << platform.name() << '\n';
	}
}

class Application {

private:
	std::string suffix, options, output{"a.out"};
	std::vector<std::string> files;

public:

	void
	arguments(int argc, char* argv[]) {
		for (int i=1; i<argc; ++i) {
			std::string arg(argv[i]);
			trim(arg, std::locale::classic());
			if (arg.compare(0, 2, "-p") == 0) {
				if (arg.size() == 2) {
					if (i+1 != argc) {
						suffix = argv[++i];
					}
				} else {
					suffix = arg.substr(2);
				}
				trim(suffix, std::locale::classic());
			} else if (arg.compare(0, 2, "-o") == 0) {
				if (arg.size() == 2) {
					if (i+1 == argc) {
						throw std::invalid_argument("bad output");
					}
					output = argv[++i];
				} else {
					output = arg.substr(2);
				}
			} else if (!arg.empty() && arg.front() == '-') {
				options += ' ';
				options += arg;
			} else {
				files.emplace_back(std::move(arg));
			}
		}
		trim(options, std::locale::classic());
	}

	void
	run() {
		if (suffix.empty()) {
			print_platforms();
			return;
		}
		if (files.empty()) {
			throw std::invalid_argument("no input files specified");
		}
		clx::platform platform;
		for (auto& p : clx::platforms()) {
			if (suffix == p.suffix()) {
				platform = std::move(p);
				break;
			}
		}
		if (!platform.get()) {
			throw std::invalid_argument("platform not found");
		}
		clx::compiler cc(platform.context());
		cc.cache(true);
		cc.devices(platform.devices(clx::device_flags::all));
		cc.options(options);
		clx::program prg = cc.compile(files.front(), catenate(files));
		std::ofstream out;
		out.exceptions(std::ios::failbit | std::ios::badbit);
		out.open(output);
		out << prg.binaries().front();
	}

};


int main(int argc, char* argv[]) {
	Application app;
	app.arguments(argc, argv);
	int ret = EXIT_FAILURE;
	try {
		app.run();
		ret = EXIT_SUCCESS;
	} catch (const std::ios::failure& err) {
		std::cerr << "i/o error" << std::endl;
	} catch (const std::exception& err) {
		std::cerr << err.what() << std::endl;
	}
	return ret;
}
