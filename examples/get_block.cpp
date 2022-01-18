/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include <xpxchaincpp/sdk.h>

#include <iostream>
#include <xpxchaincpp/model/message/message.h>
#include <chrono>
using namespace xpx_chain_sdk;

int main() {
	xpx_chain_sdk::Config config = xpx_chain_sdk::GetConfig();
	config.nodeAddress = "127.0.0.1";
	config.port = "3000";

	std::string accountAddress = "SDHDCKRVH4NXSKIRX3M7PLPMP6F4O3Z3JUWKJHLB";
	std::string publicKey = "E92978122F00698856910664C480E8F3C2FDF0A733F42970FBD58A5145BD6F21";
	std::string privateKey = "7AA907C3D80B3815BE4B4E1470DEEE8BB83BFEB330B9A82197603D09BA947230";

	auto client = xpx_chain_sdk::getClient(std::make_shared<xpx_chain_sdk::Config>(config));

//	auto score = client->blockchain()->getCurrentScore();
//	std::cout << "Chain score: " << score.scoreHigh << ' ' << score.scoreLow << std::endl;
//
//	auto storage = client->blockchain()->getStorageInfo();
//	std::cout << "Storage Info | Num Transactions: " << storage.numTransactions << std::endl;
//    std::cout << "Storage Info | Num Blocks      : " << storage.numBlocks << std::endl;
//
//	auto height = client->blockchain()->getBlockchainHeight();
//	std::cout << "Block number: " << height << std::endl;
//
//    auto block = client->blockchain()->getBlockByHeight(height);
//    std::cout << "Block signature: " << block.data.signature << std::endl;
//
//    auto blocks = client->blockchain()->getBlocksByHeightWithLimit(height - 1, 25).blocks;
//	for (auto& block: blocks) {
//	    std::cout << "Block signature: " << block.data.signature << std::endl;
//    }
//
//	std::cout << "Generation Hash: " << ' ' << client -> blockchain() -> getBlockByHeight(1).meta.generationHash << std::endl;
//
//	auto networkInfo = client -> network() -> getNetworkInfo();
//	std::cout << "Network Info " << networkInfo.description << '|' << networkInfo.name << std::endl;
//
//	auto accountInfo = client -> account() -> getAccountInfo(accountAddress);
//	std::cout << accountInfo.publicKey << ' ' << accountInfo.mosaics[0].id << std::endl;
//
//	auto mosaicId = accountInfo.mosaics[0].id;
//	std::cout << mosaicId << std::endl;
//	auto mosaicInfo = client -> mosaics() -> getMosaicInfo(mosaicId);
////
//	std::cout << "Mosaic Info: " << mosaicInfo.data.amount << std::endl;

//	std::vector<uint64_t > ids = {accountInfo.mosaics[0].id};
//
//	auto mosaicInfos = client -> mosaics() -> getMosaicInfos(ids);
//
//	std::cout << mosaicInfos.mosaics.size() << std::endl;

//	auto namespaces = client -> namespaces() -> getNamespaceInfoByAccount(accountAddress);
//	std::vector<std::string> namespaceIds;
//	for(auto x : namespaces.namespaceInfos) {
//		std::cout << x.data.owner << std::endl;
//		namespaceIds.push_back(x.meta.id);
//	}

//	auto transactions = client -> account() -> getAccountTransactions(publicKey);
//	auto transaction = reinterpret_cast<xpx_chain_sdk::transactions_info::TransferTransaction*>(transactions.transactions[0].get());
//
//	std::cout << "Transaction number: " << transactions.transactions.size() << std::endl;
//	std::cout << "Transaction version " << transaction -> version << std::endl;
//	std::cout << "Message size " << transaction -> message.size() << std::endl;
//	for(auto x : transaction -> message) {
//		std::cout<< x << ' ';
//	}
//	std::cout << std::endl;

	auto replicatorOnboardingTransaction = xpx_chain_sdk::CreateReplicatorOnboardingTransaction(2048);

	xpx_chain_sdk::Account account([privateKeyString = privateKey](PrivateKeySupplierReason reason, PrivateKeySupplierParam param) {
        Key key;
        if(reason == PrivateKeySupplierReason::Transaction_Signing) {
			ParseHexStringIntoContainer(privateKeyString.c_str(), privateKeyString.size(), key);
		}

        return PrivateKey(key.data(), key.size());
	}, replicatorOnboardingTransaction -> networkId());

	account.signTransaction(replicatorOnboardingTransaction.get());

    auto hash = ToHex(replicatorOnboardingTransaction->hash());

    auto notifier = [hash](const xpx_chain_sdk::TransactionNotification& notification) {
        if (notification.data.hash == hash) {
            std::cout << "confirmed replicatorOnboardingTransaction: " << hash << std::endl;

            exit(0);
        } else {
            std::cout << "other confirmed transaction hash: " << notification.data.hash << std::endl;
        }
    };

    client->notifications()->addConfirmedAddedNotifiers(account.address(), { notifier });

    auto statusNotifier = [](const xpx_chain_sdk::TransactionStatusNotification& notification) {
        std::cout <<  "transaction status notification is received : " << notification.status.c_str() << " : " << notification.hash.c_str() << std::endl;
    };

    client->notifications()->addStatusNotifiers(account.address(), { statusNotifier });

	try {
		client -> transactions() -> announceNewTransaction(replicatorOnboardingTransaction->binary());
	}
	catch(std::exception& e) {
		std::cout << e.what() << std::endl;
	}

    char a;
    int ret = std::scanf("%c\n", &a);
    (void) ret; // ignore
    return 0;
}