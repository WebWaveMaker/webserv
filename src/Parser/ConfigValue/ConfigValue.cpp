#include "ConfigValue.hpp"
//all test passed
// Constructors for each type

ConfigValue::ConfigValue(bool b) : type(BOOL) {
	data.boolean = b;
}

ConfigValue::ConfigValue(unsigned int i) : type(UINT) {
	data.integer = i;
}

ConfigValue::ConfigValue(const std::string& s) : type(STRING) {
	new (&data.str) std::string(s);
}

ConfigValue::ConfigValue(const std::pair<std::string, LogLevels>& l) : type(LOG) {
	new (&data.log) std::pair<std::string, LogLevels>(l);
}

ConfigValue::ConfigValue(const std::vector<HttpMethod>& v) : type(MEDVEC) {
	new (&data.medvec) std::vector<HttpMethod>(v);
}

ConfigValue::ConfigValue(const std::vector<std::string>& v) : type(STRVEC) {
	new (&data.strvec) std::vector<std::string>(v);
}

ConfigValue::ConfigValue(const ConfigValue& other) : type(other.type) {
	switch (type) {
		case NONE:
			break;
		case BOOL:
			data.boolean = other.data.boolean;
			break;
		case UINT:
			data.integer = other.data.integer;
			break;
		case STRING:
			new (&data.str) std::string(other.asString());
			break;
		case LOG:
			new (&data.log) std::pair<std::string, LogLevels>(other.asLog());
			break;
		case MEDVEC:
			new (&data.medvec) std::vector<HttpMethod>(other.asMedVec());
			break;
		case STRVEC:
			new (&data.strvec) std::vector<std::string>(other.asStrVec());
			break;
	}
}

// Destructor
ConfigValue::~ConfigValue() {
	if (type == STRING) {
		reinterpret_cast<std::string*>(&data.str)->~basic_string();
	} else if (type == LOG) {
		reinterpret_cast<std::pair<std::string, LogLevels>*>(&data.log)->~pair();
	} else if (type == MEDVEC) {
		reinterpret_cast<std::vector<HttpMethod>*>(&data.medvec)->~vector();
	} else if (type == STRVEC) {
		reinterpret_cast<std::vector<std::string>*>(&data.strvec)->~vector();
	}
}

// Assignment operators for each type
ConfigValue& ConfigValue::operator=(bool b) {
	if (type != BOOL)
		throw std::runtime_error("operator Invalid type");
	data.boolean = b;
	return *this;
}

ConfigValue& ConfigValue::operator=(unsigned int i) {
	if (type != UINT)
		throw std::runtime_error("operator Invalid type");
	data.integer = i;
	return *this;
}

ConfigValue& ConfigValue::operator=(const std::string& s) {
	if (type != STRING)
		throw std::runtime_error("operator Invalid type");
	reinterpret_cast<std::string*>(data.str)->~basic_string();
	new (data.str) std::string(s);
	return *this;
}

ConfigValue& ConfigValue::operator=(const std::pair<std::string, LogLevels>& l) {
	if (type != LOG)
		throw std::runtime_error("operator Invalid type");
	reinterpret_cast<std::pair<std::string, LogLevels>*>(data.log)->~pair();
	new (data.log) std::pair<std::string, LogLevels>(l);
	return *this;
}

ConfigValue& ConfigValue::operator=(const std::vector<HttpMethod>& v) {
	if (type != MEDVEC)
		throw std::runtime_error("operator Invalid type");
	reinterpret_cast<std::vector<HttpMethod>*>(data.medvec)->~vector();
	new (data.medvec) std::vector<HttpMethod>(v);
	return *this;
}

ConfigValue& ConfigValue::operator=(const std::vector<std::string>& v) {
	if (type != STRVEC)
		throw std::runtime_error("operator Invalid type");
	reinterpret_cast<std::vector<std::string>*>(data.strvec)->~vector();
	new (data.strvec) std::vector<std::string>(v);
	return *this;
}

ConfigValue& ConfigValue::operator=(const ConfigValue& other) {
	if (type == STRING) {
		reinterpret_cast<std::string*>(&data.str)->~basic_string();
	} else if (type == LOG) {
		reinterpret_cast<std::pair<std::string, LogLevels>*>(&data.log)->~pair();
	} else if (type == MEDVEC) {
		reinterpret_cast<std::vector<HttpMethod>*>(&data.medvec)->~vector();
	} else if (type == UINT) {
		data.integer = other.data.integer;
	} else if (type == BOOL) {
		data.boolean = other.data.boolean;
	} else if (type == STRVEC) {
		reinterpret_cast<std::vector<std::string>*>(&data.strvec)->~vector();
	} else if (type == NONE) {
	} else {
		throw std::runtime_error("operator Invalid type");
	}
	type = other.type;
	switch (type) {
		case NONE:
			break;
		case BOOL:
			data.boolean = other.data.boolean;
			break;
		case UINT:
			data.integer = other.data.integer;
			break;
		case STRING:
			new (&data.str) std::string(other.asString());
			break;
		case LOG:
			new (&data.log) std::pair<std::string, LogLevels>(other.asLog());
			break;
		case MEDVEC:
			new (&data.medvec) std::vector<HttpMethod>(other.asMedVec());
			break;
		case STRVEC:
			new (&data.strvec) std::vector<std::string>(other.asStrVec());
			break;
	}
	return *this;
}

// Getter functions

void* ConfigValue::asNULL() const {
	if (type != NONE) {
		throw std::runtime_error("Invalid type getNone");
	}
	return NULL;
}

bool ConfigValue::asBool() const {
	if (type != BOOL) {
		throw std::runtime_error("Invalid type asBool");
	}
	return data.boolean;
}

unsigned int ConfigValue::asUint() const {
	// std::cerr << type << std::endl;
	// std::cerr << data.integer << std::endl;
	if (type != UINT) {
		throw std::runtime_error("Invalid type asUint");
	}
	return data.integer;
}

std::string ConfigValue::asString() const {
	if (type != STRING) {
		throw std::runtime_error("Invalid type asString");
	}
	return *reinterpret_cast<const std::string*>(&data.str);
}

std::pair<std::string, LogLevels> ConfigValue::asLog() const {
	if (type != LOG) {
		throw std::runtime_error("Invalid type asLog");
	}
	return *reinterpret_cast<const std::pair<std::string, LogLevels>*>(&data.log);
}

std::vector<HttpMethod> ConfigValue::asMedVec() const {
	if (type != MEDVEC) {
		throw std::runtime_error("Invalid type asMedVec");
	}
	return *reinterpret_cast<const std::vector<HttpMethod>*>(&data.medvec);
}

std::vector<std::string> ConfigValue::asStrVec() const {
	if (type != STRVEC) {
		throw std::runtime_error("Invalid type asStrVec");
	}
	return *reinterpret_cast<const std::vector<std::string>*>(&data.strvec);
}

// Check the type
ConfigValue::ValueType ConfigValue::getType() const {
	return type;
}
