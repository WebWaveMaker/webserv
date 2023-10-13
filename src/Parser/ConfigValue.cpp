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
	// If the current type is STRING, then destroy the old string before placement-new
	if (type == STRING) {
		reinterpret_cast<std::string*>(data.str)->~basic_string();
	} else if (type == LOG) {
		// If the current type is LOG, then destroy the old pair before placement-new
		reinterpret_cast<std::pair<std::string, LogLevels>*>(data.log)->~pair();
	}

	// Construct the new string at the correct location
	new (data.str) std::string(s);

	// Update the type
	type = STRING;

	return *this;
}

ConfigValue& ConfigValue::operator=(const std::pair<std::string, LogLevels>& l) {
	// If the current type is STRING, then destroy the old string before placement-new
	if (type == STRING) {
		reinterpret_cast<std::string*>(data.str)->~basic_string();
	} else if (type == LOG) {
		// If the current type is LOG, then destroy the old pair before placement-new
		reinterpret_cast<std::pair<std::string, LogLevels>*>(data.log)->~pair();
	}

	// Construct the new pair at the correct location
	new (data.log) std::pair<std::string, LogLevels>(l);

	// Update the type
	type = LOG;

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
	if (type != BOOL) {
		throw std::runtime_error("Invalid type");
	}
	return data.boolean;
}

unsigned int ConfigValue::asUint() const {
	if (type != UINT) {
		throw std::runtime_error("Invalid type");
	}
	return data.integer;
}

std::string ConfigValue::asString() const {
	if (type != STRING) {
		throw std::runtime_error("Invalid type");
	}
	return *reinterpret_cast<const std::string*>(&data.str);
}

std::pair<std::string, LogLevels> ConfigValue::asLog() const {
	if (type != LOG) {
		throw std::runtime_error("Invalid type");
	}
	return *reinterpret_cast<const std::pair<std::string, LogLevels>*>(&data.log);
}

// Check the type
ConfigValue::ValueType ConfigValue::getType() const {
	return type;
}
