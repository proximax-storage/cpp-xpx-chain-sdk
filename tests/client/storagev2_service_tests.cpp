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
    auto replicatorAccount = getTestAccount(replicatorData.privateKey);
    auto driveAccount = getTestAccount(driveData.privateKey);
    auto capacity = 100;

    TEST(TEST_CLASS, StorageV2Service_Flow_Test) {
        Address replicatorAddress(replicatorData.publicKeyContainer);
        Address driveAddress(driveData.publicKeyContainer);

        Mosaic xpx(-4613020131619586570, Amount(10000));
        MosaicContainer mosaics { xpx };
        RawBuffer message("test message");

        // Transfer fund to accounts
        std::unique_ptr<TransferTransaction> driveFundTx = CreateTransferTransaction(replicatorAddress, mosaics, message);

        driveAccount->signTransaction(driveFundTx.get());
        EXPECT_TRUE(client->transactions()->announceNewTransaction(driveFundTx->binary()));

        std::unique_ptr<TransferTransaction> replicatorFundTx = CreateTransferTransaction(driveAddress, mosaics, message);

        driveAccount->signTransaction(replicatorFundTx.get());
        EXPECT_TRUE(client->transactions()->announceNewTransaction(replicatorFundTx->binary()));

        // Replicator onboarding transaction
        std::unique_ptr<ReplicatorOnboardingTransaction> onboardingTx = CreateReplicatorOnboardingTransaction(
                capacity);

        replicatorAccount->signTransaction(onboardingTx.get());

        EXPECT_TRUE(client->transactions()->announceNewTransaction(onboardingTx->binary()));

        sleep(5);

        Replicator replicatorInfo = client->storagev2()->getReplicatorByPublicKey(replicatorData.publicKey);
        EXPECT_EQ(replicatorData.publicKey, replicatorInfo.replicatorKey);
        EXPECT_EQ(capacity, replicatorInfo.capacity);
        EXPECT_EQ(0, replicatorInfo.drives.size());

        // PrepareDrive transaction
        std::unique_ptr<PrepareBcDriveTransaction> prepareDriveTx = CreatePrepareBcDriveTransaction(
                100, Amount(10), 1);

        driveAccount->signTransaction(prepareDriveTx.get());

        EXPECT_TRUE(client->transactions()->announceNewTransaction(prepareDriveTx->binary()));
        
        // sleep(5);

        // BcDrive bcDriveInfo = client->storagev2()->getBcDriveByAccountId(driveData.publicKey);
        // EXPECT_EQ(driveData.publicKey, bcDriveInfo.owner);
        
        // DriveClosure transaction
        std::unique_ptr<DriveClosureTransaction> driveClosureTx = CreateDriveClosureTransaction(prepareDriveTx.get()->hash());

        driveAccount->signTransaction(driveClosureTx.get()); 

        EXPECT_TRUE(client->transactions()->announceNewTransaction(driveClosureTx->binary()));

        sleep(10);

        EXPECT_ANY_THROW(client->storagev2()->getBcDriveByAccountId(driveData.publicKey));

        // Replicator offboarding transaction
        std::unique_ptr<ReplicatorOffboardingTransaction> offboardingTx = CreateReplicatorOffboardingTransaction();

        replicatorAccount->signTransaction(offboardingTx.get());

        EXPECT_TRUE(client->transactions()->announceNewTransaction(offboardingTx->binary()));

        sleep(10);

        EXPECT_ANY_THROW(client->storagev2()->getReplicatorByPublicKey(replicatorData.publicKey));
    }

}
