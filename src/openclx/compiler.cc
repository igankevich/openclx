#include <dirent.h>

#include <fstream>
#include <iomanip>
#include <sstream>

#include <openclx/binary>
#include <openclx/compiler>
#include <openclx/kernel>
#include <openclx/platform>

#define CLX_SYS_THROW() \
    throw ::std::system_error(errno, ::std::system_category())

#define CLX_SYS_CHECK(ret) \
    if (ret == -1) { \
        CLX_SYS_THROW(); \
    }

namespace {
    typedef ::DIR directory;
    typedef struct ::dirent directory_entry;
    constexpr const char file_separator = '/';
    constexpr const int max_includes = 1000;
}

std::string
clx::compiler::inline_sources(std::string src) const {
    int nincludes = 0;
    std::string include = "#include";
    for (size_t offset=0;
         offset!=std::string::npos && offset<src.size() && nincludes<max_includes;
         offset=src.find(include, offset)) {
        size_t i=offset+include.size();
        // skip spaces
        for (; i<src.size() && std::isspace(src[i]); ++i);
        if (i < src.size()) {
            char s = '"';
            if (src[i] == '"') { s = '"'; }
            else if (src[i] == '<') { s = '>'; }
            else continue;
            size_t j = i+1;
            // find closing quote
            for (; j<src.size() && src[j]!=s && (s=='>' || (s=='"' && src[j-1]!='\\')); ++j);
            if (j < src.size()) { src += file_to_string(src.substr(i+1,j-i-1)); ++nincludes; }
        }
        offset = i;
    }
    return src;
}

std::string
clx::compiler::file_to_string(const std::string& filename) const {
    const auto& src = this->source_directory();
    const auto& suffix = this->suffix();
    std::string path;
    path.reserve(filename.size() + suffix.size() + src.size() + 1);
    path += src;
    path += file_separator;
    path += filename;
    path += suffix;
    std::stringstream tmp;
    std::ifstream in;
    in.exceptions(std::ios::failbit | std::ios::badbit);
    in.open(path, std::ios::in);
    tmp << in.rdbuf();
    return tmp.str();
}

std::string
clx::compiler::binary_path(
    const std::string& filename,
    hash_type::result_type number
) const {
    std::stringstream name;
    name << this->cache_directory();
    name << file_separator;
    name << filename << '-';
    name << std::setfill('0') << std::setw(2*sizeof(number)) << std::hex << number;
    return name.str();
}

clx::program
clx::compiler::compile(const std::string& filename) {
    program prg;
    this->compile(prg, filename, file_to_string(filename));
    return prg;
}

clx::program
clx::compiler::compile(const std::string& filename, const std::string& src) {
    program prg;
    this->compile(prg, filename, src);
    return prg;
}

void
clx::compiler::compile(
    program& prg,
    const std::string& filename,
    const std::string& src
) {
    if (cache()) {
        hash_type hash;
        const auto& devices = this->devices();
        try {
            std::vector<binary> binaries;
            binaries.reserve(devices.size());
            for (const auto& device : devices) {
                auto key = hash(salt(device) + inline_sources(src));
                const auto& path = binary_path(filename, key);
                std::ifstream in;
                in.exceptions(std::ios::failbit | std::ios::badbit);
                in.open(path, std::ios::binary | std::ios::in);
                binaries.emplace_back();
                in >> binaries.back();
            }
            prg = this->_context.program(binaries);
            prg.build(options(), devices);
        } catch (const std::exception& err) {
            this->compile_from_source(prg, src);
            const auto& binaries = prg.binaries();
            const auto ndevices = devices.size();
            for (size_t i=0; i<ndevices; ++i) {
                const auto& device = devices[i];
                const auto& binary = binaries[i];
                auto key = hash(salt(device) + inline_sources(src));
                const auto& path = binary_path(filename, key);
                std::ofstream out;
                out.exceptions(std::ios::failbit | std::ios::badbit);
                out.open(path, std::ios::binary | std::ios::out);
                out << binary;
                out.close();
            }
        }
    } else {
        this->compile_from_source(prg, src);
    }
}

void
clx::compiler::compile_from_source(program& prg, const std::string& src) {
    prg = this->_context.program(src);
    try {
        prg.build(options(), devices());
    } catch (const std::system_error& err) {
        if (errc(err.code().value()) == errc::build_program_failure) {
            throw std::system_error(err.code(), this->errors(prg));
        }
        throw;
    }
}

void
clx::compiler::compile() {
    directory* dir = ::opendir(this->_source_directory.data());
    if (!dir) { CLX_SYS_THROW(); }
    directory_entry* entry;
    const auto& suffix = this->suffix();
    const auto n = suffix.size();
    while ((entry = ::readdir(dir))) {
        std::string filename = entry->d_name;
        if (filename.size() < n ||
            filename.compare(filename.size()-n, n, suffix) != 0) {
            continue;
        }
        this->compile(entry->d_name);
    }
    CLX_SYS_CHECK(::closedir(dir));
}

void
clx::compiler::clear_cache() {
    directory* dir = ::opendir(this->_cache_directory.data());
    if (!dir) { CLX_SYS_THROW(); }
    directory_entry* entry;
    while ((entry = ::readdir(dir))) {
        std::string path;
        path += source_directory();
        path += file_separator;
        path += entry->d_name;
        std::remove(path.data());
    }
    CLX_SYS_CHECK(::closedir(dir));
}

std::string
clx::compiler::errors(const program& prg) const {
    std::string logs;
    logs.reserve(4096);
    for (const auto& device : devices()) {
        const auto& log = prg.build_log(device);
        if (!log.empty()) {
            logs += log;
            logs += '\n';
        }
    }
    return logs;
}

void
clx::compiler::devices(const device_array& rhs) {
    this->_devices = rhs;
}

void
clx::compiler::devices(device_array&& rhs) {
    this->_devices = std::move(rhs);
}

std::string
clx::compiler::salt(const device& device) const {
    std::string salt;
    salt.reserve(4096);
    const auto& platform = device.platform();
    salt += platform.name();
    salt += platform.vendor();
    salt += platform.version();
    salt += device.name();
    salt += device.vendor();
    salt += device.version();
    return salt;
}

void
clx::compiler::unload() {
    auto& devices = this->_devices;
    for (const auto& device : devices) {
        device.platform().unload_compiler();
    }
}

::clx::kernel clx::compiler::kernel(
    const std::string& filename,
    const char* name
) {
    return this->compile(filename).kernel(name);
}

