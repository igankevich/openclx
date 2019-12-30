#include <fstream>
#include <iomanip>
#include <iostream>
#include <locale>
#include <sstream>
#include <string>

#include <openclx/compiler>
#include <openclx/context>
#include <openclx/extensions>
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
            << "    " << platform.name()
            << '\n';
    }
}

void
option(const char* version, const char* category, const char* option) {
    std::cout << std::setw(20) << std::right << category << "    ";
    std::cout << std::setw(40) << std::left << option;
    std::cout << std::setw(7) << std::right << version;
    std::cout << '\n';
}

void
usage() {
    std::cout << "usage: clx [-p platform] [-o filename] [-h] [options] file...\n";
    std::cout << "supported platforms:\n";
    print_platforms();
    std::cout << "compiler options:\n";
    option("1.0", "math", "-cl-single-precision-constant");
    option("1.0", "", "-cl-denorms-are-zero");
    option("1.2", "", "-cl-fp32-correctly-rounded-divide-sqrt");
    option("1.0", "optimisation", "-cl-opt-disable");
    option("1.0", "", "-cl-strict-aliasing");
    option("1.0", "", "-cl-mad-enable");
    option("1.0", "", "-cl-no-signed-zeros");
    option("1.0", "", "-cl-unsafe-math-optimizations");
    option("1.0", "", "-cl-finite-math-only");
    option("1.0", "", "-cl-fast-relaxed-math");
    option("2.0", "", "-cl-uniform-work-group-size");
    option("2.2", "", "-cl-no-subgroup-ifp");
    option("1.0", "preprocesor", "-D -I");
    option("1.0", "warnings", "-w -Werror");
    option("1.1", "version", "-cl-std=CL2.0");
    option("1.2", "kernels", "-cl-kernel-arg-info");
    option("2.0", "debugging", "-g");
    option("2.0", "spir", "-x spir -spir-std=1.2");
    option("", "nvidia", "-cl-nv-maxrregcount N");
    option("", "", "-cl-nv-opt-level N");
    option("", "", "-cl-nv-verbose");
    option("", "arm", "-cl-arm-non-uniform-work-group-size");
    option("", "", "-cl-arm-svm");
    std::cout << "linker options:\n";
    option("1.2", "library", "-create-library");
    option("1.2", "", "-enable-link-options");
    option("1.2", "programme", "-cl-denorms-are-zero");
    option("1.2", "", "-cl-no-signed-zeroes");
    option("1.2", "", "-cl-unsafe-math-optimizations");
    option("1.2", "", "-cl-finite-math-only");
    option("1.2", "", "-cl-fast-relaxed-math");
    option("2.2", "", "-cl-no-subgroup-ifp");
    std::cout << "Please, refer to OpenCL standard "
        "for the full list of supported compiler options.\n";
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
            usage();
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
        clx::compiler cc{clx::context{platform}};
        cc.cache(true);
        cc.devices(platform.devices(clx::device_flags::all));
        cc.options(options);
        clx::program prg = cc.compile(files.front(), catenate(files));
        std::ofstream out;
        out.exceptions(std::ios::failbit | std::ios::badbit);
        out.open(output);
        out << prg.binaries().front();
        out.close();
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
