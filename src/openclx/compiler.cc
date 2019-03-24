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
	name << this->binary_directory();
	name << file_separator;
	name << filename << '-';
	name << std::setfill('0') << std::setw(sizeof(number)) << std::hex << number;
	return name.str();
}

clx::program
clx::compiler::compile(const std::string& filename) {
	auto& programs = this->_programs;
	program_map::iterator it; bool success;
	std::tie(it, success) = programs.insert({filename,{}});
	if (!success) {
		return it->second;
	}
	const auto& src = file_to_string(filename);
	auto& prg = it->second;
	if (cache()) {
		hash_type hash;
		auto key = hash(salt() + src);
		const auto& path = binary_path(filename, key);
		std::fstream stream;
		try {
			stream.exceptions(std::ios::failbit | std::ios::badbit);
			stream.open(path, std::ios::binary | std::ios::in);
			binary bin; stream >> bin;
			prg = this->_context.program(bin);
			prg.build(options(), devices());
			stream.clear();
			stream.close();
			stream.open(path, std::ios::binary | std::ios::out);
			stream << bin;
			stream.close();
		} catch (const std::exception& err) {
			this->compile_from_source(prg, src);
		}
	} else {
		this->compile_from_source(prg, src);
	}
	return prg;
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
	directory* dir = ::opendir(this->_source_directory.data());
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
	this->_programs.clear();
	this->salts();
}

void
clx::compiler::salts() {
	auto& devices = this->_devices;
	auto& salt = this->_salt;
	salt.clear();
	salt.reserve(4096);
	for (const auto& device : devices) {
		const auto& platform = device.platform();
		salt += platform.name();
		salt += platform.vendor();
		salt += platform.version();
		salt += device.name();
		salt += device.vendor();
		salt += device.version();
	}
	salt.shrink_to_fit();
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

