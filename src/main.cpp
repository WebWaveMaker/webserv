/**
 * @file main.cpp
 * @brief
 * @author WebWaveMaker
 * @version 0.1
 * @date 2023-10-04
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <cerrno>
#include "ConfigParser.hpp"
#include "Dispatcher.hpp"
#include "ServerConfig.hpp"
#include "ServerManager.hpp"

int main(int ac, char** av) {
	if (ac != 2)
		return ErrorLogger::parseError(__FILE__, __LINE__, __func__, "wrong arguments");
	try {
		ServerManager::getInstance()->init(av[1]);

		while (true)
			reactor::Dispatcher::getInstance()->handleEvent();
	} catch (...) {
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
// #include <iostream>
// #include "LocationConfig.hpp"
// #include "ServerConfig.hpp"

// int main() {
// 	try {
// 		// Dynamically create a ServerConfig to manage with shared_ptr
// 		utils::shared_ptr<ServerConfig> serverConfig(new ServerConfig());

// 		// Create LocationConfigs with serverConfig as the parent
// 		utils::shared_ptr<LocationConfig> locationConfig1(new LocationConfig(serverConfig));
// 		utils::shared_ptr<LocationConfig> locationConfig2(new LocationConfig(serverConfig));

// 		// Set some directives for the locations
// 		locationConfig1->setDirectives("root", std::vector<std::string>(1, "/var/www/html"));
// 		locationConfig1->setDirectives("index", std::vector<std::string>(1, "index.html"));
// 		locationConfig2->setDirectives("root", std::vector<std::string>(1, "/var/www/images"));
// 		locationConfig2->setDirectives("index", std::vector<std::string>(1, "index.png"));

// 		// Add the locations to the server config
// 		serverConfig->setLocations("/", locationConfig1);
// 		serverConfig->setLocations("/images", locationConfig2);

// 		// Test the getLocationConfig() function with different paths
// 		utils::shared_ptr<LocationConfig> resultConfig;

// 		// Test with root path
// 		resultConfig = serverConfig->getLocationConfig("/");
// 		std::cout << "Root path config root: " << resultConfig->getDirectives(ROOT).asString() << std::endl;

// 		// Test with images path
// 		resultConfig = serverConfig->getLocationConfig("/images/someimage.png");
// 		std::cout << "Images path config root: " << resultConfig->getDirectives(ROOT).asString() << std::endl;

// 		// Test with a non-existent path
// 		resultConfig = serverConfig->getLocationConfig("/nonexistent/path");
// 		std::cout << "Non-existent path config root: " << resultConfig->getDirectives(ROOT).asString() << std::endl;

// 	} catch (const std::exception& e) {
// 		std::cerr << "Exception caught: " << e.what() << std::endl;
// 	}

// 	return 0;
// }

// 이 테스트 코드는 ServerConfig와 LocationConfig 클래스의 객체들을 생성하고,
// 	이를 이용하여 getLocationConfig() 함수가 올바르게 동작하는지 확인하는 것입니다
// 			.아래는 코드의 주요 단계별 설명입니다 :

// 	ServerConfig 객체를 동적으로 할당하여
// 	shared_ptr를 통해 메모리 관리를 합니다.이 객체는 웹 서버의 설정을 담고
// 	있습니다.

// 	serverConfig를 부모로 사용하여 두 개의 LocationConfig 객체를
// 	생성합니다.LocationConfig 객체들은 특정 경로에 대한 웹 서버의 위치 설정을 담고
// 	있습니다.

// 	이 두 LocationConfig 객체에 각각 다른 루트 디렉토리를 지정합니다.첫 번째는
// 		/ var / www / html 경로를,
// 	두 번째는 / var / www /
// 		images 경로를 루트로 설정합니다.또한 각 위치에 대한 인덱스 파일도 설정합니다
// 			.

// 		이렇게 설정된 LocationConfig 객체들을 ServerConfig 객체에 추가합니다.첫 번째 위치는 루트 경로(/) 에,
// 	두 번째 위치는 /
// 		images 경로에 매핑됩니다
// 			.

// 		getLocationConfig() 함수를 호출하여 다양한 요청 경로에 대한 설정을 조회합니다
// 			.먼저 루트 경로(/) 에 대해 호출하여 해당하는 LocationConfig 객체의 루트 디렉토리가 출력되는지 확인합니다.

// 		/ images / someimage.png 경로에 대해 getLocationConfig() 함수를 호출하여 /
// 		images 경로에 설정된 LocationConfig 객체를 조회합니다.이 객체의 루트 디렉토리가 출력되는지 확인합니다
// 			.

// 		존재하지 않는 경로(/ nonexistent / path) 에 대해 함수를 호출하여 기본 루트 설정이 반환되는지
// 		확인합니다.이 경우 첫 번째 LocationConfig 객체의 루트 디렉토리가 출력되어야 합니다
// 			.

// 		마지막으로 프로그램이 종료될 때 객체들이 소멸자를 통해 올바르게 메모리 해제를 수행하는지
// 		확인합니다.만약 메모리 해제 과정에서 문제가 발생하면,
// 	적절한 오류 메시지와 함께 프로그램이 비정상 종료될 것입니다.
