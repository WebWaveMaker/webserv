#include "ConfigValue.hpp"

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

ConfigValue::ConfigValue(const std::vector<HttpMethods>& v) : type(MEDVEC) {
	new (&data.medvec) std::vector<HttpMethods>(v);
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
			new (&data.medvec) std::vector<HttpMethods>(other.asMedVec());
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
		reinterpret_cast<std::vector<HttpMethods>*>(&data.medvec)->~vector();
	} else if (type == STRVEC) {
		reinterpret_cast<std::vector<std::string>*>(&data.strvec)->~vector();
	}
}

// Assignment operators for each type
ConfigValue& ConfigValue::operator=(bool b) {
	if (type != BOOL)
		throw ErrorLogger::parseError(__FILE__, __LINE__, __func__, "operator Invalid type");
	data.boolean = b;
	return *this;
}

ConfigValue& ConfigValue::operator=(unsigned int i) {
	if (type != UINT)
		throw ErrorLogger::parseError(__FILE__, __LINE__, __func__, "operator Invalid type");
	data.integer = i;
	return *this;
}

ConfigValue& ConfigValue::operator=(const std::string& s) {
	if (type != STRING)
		throw ErrorLogger::parseError(__FILE__, __LINE__, __func__, "operator Invalid type");
	reinterpret_cast<std::string*>(data.str)->~basic_string();
	new (data.str) std::string(s);
	return *this;
}

ConfigValue& ConfigValue::operator=(const std::pair<std::string, LogLevels>& l) {
	if (type != LOG)
		throw ErrorLogger::parseError(__FILE__, __LINE__, __func__, "operator Invalid type");
	reinterpret_cast<std::pair<std::string, LogLevels>*>(data.log)->~pair();
	new (data.log) std::pair<std::string, LogLevels>(l);
	return *this;
}

ConfigValue& ConfigValue::operator=(const std::vector<HttpMethods>& v) {
	if (type != MEDVEC)
		throw ErrorLogger::parseError(__FILE__, __LINE__, __func__, "operator Invalid type");
	reinterpret_cast<std::vector<HttpMethods>*>(data.medvec)->~vector();
	new (data.medvec) std::vector<HttpMethods>(v);
	return *this;
}

ConfigValue& ConfigValue::operator=(const std::vector<std::string>& v) {
	if (type != STRVEC)
		throw ErrorLogger::parseError(__FILE__, __LINE__, __func__, "operator Invalid type");
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
		reinterpret_cast<std::vector<HttpMethods>*>(&data.medvec)->~vector();
	} else if (type == UINT) {
		data.integer = other.data.integer;
	} else if (type == BOOL) {
		data.boolean = other.data.boolean;
	} else if (type == STRVEC) {
		reinterpret_cast<std::vector<std::string>*>(&data.strvec)->~vector();
	} else if (type == NONE) {
	} else {
		throw ErrorLogger::parseError(__FILE__, __LINE__, __func__, "operator Invalid type");
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
			new (&data.medvec) std::vector<HttpMethods>(other.asMedVec());
			break;
		case STRVEC:
			new (&data.strvec) std::vector<std::string>(other.asStrVec());
			break;
	}
	return *this;
}

void* ConfigValue::asNULL() const {
	if (type != NONE) {
		throw ErrorLogger::parseError(__FILE__, __LINE__, __func__, "Invalid type asNULL");
	}
	return NULL;
}

bool ConfigValue::asBool() const {
	if (type != BOOL) {
		throw ErrorLogger::parseError(__FILE__, __LINE__, __func__, "Invalid type asBool");
	}
	return data.boolean;
}

unsigned int ConfigValue::asUint() const {
	if (type != UINT) {
		throw ErrorLogger::parseError(__FILE__, __LINE__, __func__, "Invalid type asUint");
	}
	return data.integer;
}

std::string ConfigValue::asString() const {
	if (type != STRING) {
		throw ErrorLogger::parseError(__FILE__, __LINE__, __func__, "Invalid type asString");
	}
	return *reinterpret_cast<const std::string*>(&data.str);
}

std::pair<std::string, LogLevels> ConfigValue::asLog() const {
	if (type != LOG) {
		throw ErrorLogger::parseError(__FILE__, __LINE__, __func__, "Invalid type asLog");
	}
	return *reinterpret_cast<const std::pair<std::string, LogLevels>*>(&data.log);
}

std::vector<HttpMethods> ConfigValue::asMedVec() const {
	if (type != MEDVEC) {
		throw ErrorLogger::parseError(__FILE__, __LINE__, __func__, "Invalid type asMedVec");
	}
	return *reinterpret_cast<const std::vector<HttpMethods>*>(&data.medvec);
}

std::vector<std::string> ConfigValue::asStrVec() const {
	if (type != STRVEC) {
		throw ErrorLogger::parseError(__FILE__, __LINE__, __func__, "Invalid type asStrVec");
	}
	return *reinterpret_cast<const std::vector<std::string>*>(&data.strvec);
}

// Check the type
ConfigValue::ValueType ConfigValue::getType() const {
	return type;
}
