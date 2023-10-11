#include "ConfigValue.hpp"
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

ConfigValue::ConfigValue(const ConfigValue& other) : type(other.type) {
	switch (type) {
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
	}
}

// Destructor
ConfigValue::~ConfigValue() {
	if (type == STRING) {
		reinterpret_cast<std::string*>(&data.str)->~basic_string();
	} else if (type == LOG) {
		reinterpret_cast<std::pair<std::string, LogLevels>*>(&data.log)->~pair();
	}
}

// Assignment operators for each type
ConfigValue& ConfigValue::operator=(bool b) {
	if (type == STRING) {
		asString().~basic_string();
	} else if (type == LOG) {
		asLog().~pair();
	}
	type = BOOL;
	data.boolean = b;
	return *this;
}

ConfigValue& ConfigValue::operator=(unsigned int i) {
	if (type == STRING) {
		asString().~basic_string();
	} else if (type == LOG) {
		asLog().~pair();
	}
	type = UINT;
	data.integer = i;
	return *this;
}

ConfigValue& ConfigValue::operator=(const std::string& s) {
	if (type == STRING) {
		asString() = s;
	} else {
		if (type == LOG) {
			asLog().~pair();
		}
		new (&data.str) std::string(s);
		type = STRING;
	}
	return *this;
}

ConfigValue& ConfigValue::operator=(const std::pair<std::string, LogLevels>& l) {
	if (type == LOG) {
		asLog() = l;
	} else {
		if (type == STRING) {
			asString().~basic_string();
		}
		new (&data.log) std::pair<std::string, LogLevels>(l);
		type = LOG;
	}
	return *this;
}

ConfigValue& ConfigValue::operator=(const ConfigValue& other) {
	if (this != &other) {
		if (type == STRING) {
			asString().~basic_string();	 // here is line 97
		} else if (type == LOG) {
			asLog().~pair();
		}

		type = other.type;
		switch (type) {
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
		}
	}
	return *this;
}

// Getter functions
bool ConfigValue::asBool() const {
	return data.boolean;
}

unsigned int ConfigValue::asUint() const {
	return data.integer;
}

std::string ConfigValue::asString() const {
	return *reinterpret_cast<const std::string*>(&data.str);
}

std::pair<std::string, LogLevels> ConfigValue::asLog() const {
	return *reinterpret_cast<const std::pair<std::string, LogLevels>*>(&data.log);
}

// Check the type
ConfigValue::ValueType ConfigValue::getType() const {
	return type;
}
