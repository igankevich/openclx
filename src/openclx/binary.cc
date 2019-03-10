#include <ostream>
#include <sstream>

#include <openclx/binary>

void
clx::binary::write(std::ostream& out) const {
	out.write(
		reinterpret_cast<const char*>(this->data()),
		this->size()
	);
}

void
clx::binary::read(std::istream& in) {
	this->_bytes.clear();
	std::stringstream tmp;
	tmp << in.rdbuf();
	const auto& str = tmp.str();
	this->_bytes.assign(str.begin(), str.end());
}

std::ostream&
clx::operator<<(std::ostream& out, const binary& rhs) {
	rhs.write(out); return out;
}

std::istream&
clx::operator>>(std::istream& in, binary& rhs) {
	rhs.read(in); return in;
}

