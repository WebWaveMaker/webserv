#include "SignUp.hpp"

PostResponseBuilder::SignUp::SignUp(PostResponseBuilder* builder) {
	std::string& body = builder->_request->second.getBody();

    std::vector<std::string> vec = utils::split(body, "&");

    for (std::vector<std::string>::const_iterator cit = vec.begin(); cit != vec.end(); ++cit) {
        std::vector<std::string> vec2 = utils::split(*cit, "=");
        if (vec2.size() != 2)
            throw utils::shared_ptr<IBuilder<reactor::sharedData_t> >(
                new ErrorResponseBuilder(BAD_REQUEST, builder->_sharedData, builder->_serverConfig, builder->_locationConfig));
    }
}

PostResponseBuilder::SignUp::~SignUp() {}

void PostResponseBuilder::SignUp::signup() {
	std::cout << "PostResponseBuilder::SignUp::signup()" << std::endl;
}