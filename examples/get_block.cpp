#include <nemcpp/sdk.h>

#include <iostream>


int main() {
	xpx_sdk::Config config = xpx_sdk::GetConfig();
	config.nodeAddress = "bcstage1.xpxsirius.io";
	config.port = "3000";

	std::string accountAddress = "VA7PKVZYTGLHZUCZTIM6TCJZIW2KB2PYCMKVTF27";
	std::string publicKey = "E0C0BDFD0CFBC83D5DDC5F16CAD9CF18FE4339649946A79AC334AB5AA39D4BB7";

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

	auto transactions = client -> account() -> getAccountTransactions(publicKey);
	auto transaction = reinterpret_cast<xpx_sdk::transactions_info::TransferTransaction*>(transactions.transactions[0].get());

	std::cout << "Transaction number: " << transactions.transactions.size() << std::endl;
	std::cout << "Transaction version " << transaction -> version << std::endl;
	std::cout << "Message size " << transaction -> message.size() << std::endl;
	for(auto x : transaction -> message) {
		std::cout<< x << ' ';
	}
	std::cout << std::endl;

	xpx_sdk::MosaicPropertyContainer propertyContainer;

	propertyContainer.insert(xpx_sdk::MosaicProperty{xpx_sdk::MosaicPropertyId::Divisibility, 12});
	propertyContainer.insert(xpx_sdk::MosaicProperty{xpx_sdk::MosaicPropertyId::Flags, 12});

	xpx_sdk::MosaicProperties mosaicProperties(propertyContainer);
	auto result = xpx_sdk::CreateMosaicDefinitionTransaction(0, 182347912384723ll, mosaicProperties);


	try {
		auto response = client -> transactions() -> announceNewTransaction(result -> binary());
		std::cout << response << std::endl;
	}
	catch(std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}