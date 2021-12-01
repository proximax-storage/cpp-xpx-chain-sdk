/**
*** Copyright 2021 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/

#include <gtest/gtest.h>
#include "../config.h"

namespace xpx_chain_sdk::tests {

#define TEST_CLASS StorageV2Service

    ClientData replicatorData;
    ClientData2 driveData;
    auto client = xpx_chain_sdk::getClient(std::make_shared<xpx_chain_sdk::Config>(getTestConfiguration()));
    auto replicatorAccount = getTestAccount(clientData.privateKey);
    auto driveAccount = getTestAccount(driveData.privateKey);

    TEST(TEST_CLASS, StorageV2Service_Flow_Test) {
        Address replicatorAddress(clientData.publicKeyContainer);
        Address driveAddress(driveData.publicKeyContainer);

        // srand(time(0));
        // uint32_t nonce = rand();
        // Mosaic mosaic;
        // mosaic.id = mosaic.GenerateId(replicatorData.publicKeyContainer, nonce);
        // MosaicProperties mosaicProperties(MosaicFlags::All, 4);

        // std::unique_ptr<MosaicDefinitionTransaction> mosaicDefinitionTx =  
        // CreateMosaicDefinitionTransaction(
        //         nonce, mosaic.id, MosaicFlags::All, mosaicProperties);
        
        // driveAccount->signTransaction(mosaicDefinitionTx.get());
       
        // EXPECT_TRUE(client->transactions()->announceNewTransaction(mosaicDefinitionTx->binary()));

        // Replicator onboarding transaction
        std::unique_ptr<ReplicatorOnboardingTransaction> onboardingTx = CreateReplicatorOnboardingTransaction(
                10, BLSPublicKey());

        replicatorAccount->signTransaction(onboardingTx.get());

        EXPECT_TRUE(client->transactions()->announceNewTransaction(onboardingTx->binary()));

        sleep(10);

        // PrepareDrive transaction
        std::unique_ptr<PrepareBcDriveTransaction> prepareDriveTx = CreatePrepareBcDriveTransaction(
                100, Amount(10), 1);

        driveAccount->signTransaction(prepareDriveTx.get());

        EXPECT_TRUE(client->transactions()->announceNewTransaction(prepareDriveTx->binary()));
        
        sleep(10);
        
        // DriveClosure transaction
        std::unique_ptr<DriveClosureTransaction> driveClosureTx = CreateDriveClosureTransaction(prepareDriveTx.get()->hash());

        driveAccount->signTransaction(driveClosureTx.get()); 

        EXPECT_TRUE(client->transactions()->announceNewTransaction(driveClosureTx->binary()));

        sleep(10);

        // Replicator offboarding transaction
        std::unique_ptr<ReplicatorOffboardingTransaction> offboardingTx = CreateReplicatorOffboardingTransaction();

        replicatorAccount->signTransaction(offboardingTx.get());

        EXPECT_TRUE(client->transactions()->announceNewTransaction(offboardingTx->binary()));
    }

}
