#include <nemcpp/sdk.h>

#include <iostream>

int main() {
	nem2_sdk::Config config = nem2_sdk::GetConfig();
	config.nodeAddress = "bcstage2.xpxsirius.io";
	config.port = "3000";

	nem2_sdk::Client client(std::make_shared<nem2_sdk::Config>(config));

	std::cout << client.blockchain()->getBlockchainHeight() << std::endl;
}
