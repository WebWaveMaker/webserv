#include "Mime.hpp"

Mime::Mime() {}

Mime::Mime(const Mime& other) {
	*this = other;
}

Mime::~Mime() {}

Mime& Mime::operator=(const Mime& other) {
	if (this != &other) {
		_mimeTypes = other._mimeTypes;
	}
	return *this;
}
//extension = html mimeType = text/html
void Mime::setMimeTypes(std::string extension, std::string mimeType) {
	//remove the dot
	if (extension[0] == '.') {
		extension.erase(0, 1);
	}
	_mimeTypes[extension] = mimeType;
}

std::string Mime::getMimeType(const std::string& extension) const {
	std::map<std::string, std::string>::const_iterator it = _mimeTypes.find(extension);
	if (it != _mimeTypes.end()) {
		return it->second;
	}
	return "application/octet-stream";
}