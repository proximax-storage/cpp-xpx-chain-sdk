#include <nemcpp/sdk.h>

#include <iostream>

int main() {
	nem2_sdk::Config config = nem2_sdk::GetConfig();
	config.nodeAddress = "bcstage1.xpxsirius.io";
	config.port = "3000";

	auto client = nem2_sdk::getClient(std::make_shared<nem2_sdk::Config>(config));

	auto score = client->blockchain()->getCurrentScore();
	std::cout << "Chain score: " << score.scoreHigh << ' ' << score.scoreLow << std::endl;

	auto storage = client->blockchain()->getStorageInfo();
	std::cout << "Storage Info | Num Transactions: " << storage.numTransactions << std::endl;
    std::cout << "Storage Info | Num Blocks      : " << storage.numBlocks << std::endl;

	auto height = client->blockchain()->getBlockchainHeight();
	std::cout << "Block number: " << height << std::endl;

    auto block = client->blockchain()->getBlockByHeight(height);
    std::cout << "Block signature: " << block.data.signature << std::endl;

    auto blocks = client->blockchain()->getBlocksByHeightWithLimit(height - 1, 25);
	for (auto& block: blocks) {
	    std::cout << "Block signature: " << block.data.signature << std::endl;
    }
}
