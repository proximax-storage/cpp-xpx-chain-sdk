#include <nemcpp/sdk.h>

#include <iostream>

int main() {
	xpx_sdk::Config config = xpx_sdk::GetConfig();
	config.nodeAddress = "bcstage1.xpxsirius.io";
	config.port = "3000";

//	config.nodeAddress = "192.168.88.41";

	std::string accountAddress = "VA7PKVZYTGLHZUCZTIM6TCJZIW2KB2PYCMKVTF27";
	accountAddress = "SBGS2IGUED476REYI5ZZGISVSEHAF6YIQZV6YJFQ";

	auto client = xpx_sdk::getClient(std::make_shared<xpx_sdk::Config>(config));

	auto score = client->blockchain()->getCurrentScore();
	std::cout << "Chain score: " << score.scoreHigh << ' ' << score.scoreLow << std::endl;

	auto storage = client->blockchain()->getStorageInfo();
	std::cout << "Storage Info | Num Transactions: " << storage.numTransactions << std::endl;
    std::cout << "Storage Info | Num Blocks      : " << storage.numBlocks << std::endl;

	auto height = client->blockchain()->getBlockchainHeight();
	std::cout << "Block number: " << height << std::endl;

    auto block = client->blockchain()->getBlockByHeight(height);
    std::cout << "Block signature: " << block.data.signature << std::endl;

    auto blocks = client->blockchain()->getBlocksByHeightWithLimit(height - 1, 25).blocks;
	for (auto& block: blocks) {
	    std::cout << "Block signature: " << block.data.signature << std::endl;
    }

	auto networkInfo = client -> network() -> getNetworkInfo();
	std::cout << "Network Info " << networkInfo.description << ' ' << networkInfo.name << std::endl;

	auto accountInfo = client -> account() -> getAccountInfo(accountAddress);
	std::cout << accountInfo.publicKey << ' ' << accountInfo.mosaics[0].id << std::endl;

	auto mosaicId = accountInfo.mosaics[0].id;
	std::cout << mosaicId << std::endl;
	auto mosaicInfo = client -> mosaics() -> getMosaicInfo(mosaicId);
//
	std::cout << "Mosaic Info: " << mosaicInfo.data.amount << std::endl;

	std::vector<uint64_t > ids = {accountInfo.mosaics[0].id};

	auto mosaicInfos = client -> mosaics() -> getMosaicInfos(ids);

	std::cout << mosaicInfos.mosaics.size() << std::endl;

	auto namespaces = client -> namespaces() -> getNamespaceInfoByAccount(accountAddress);
	std::vector<std::string> namespaceIds;
	for(auto x : namespaces.namespaceInfos) {
		std::cout << x.data.owner << std::endl;
		namespaceIds.push_back(x.meta.id);
	}

}
