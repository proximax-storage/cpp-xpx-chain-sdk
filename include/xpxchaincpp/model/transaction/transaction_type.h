/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <cstdint>

namespace xpx_chain_sdk {
	
	/// Transaction type.
    enum class TransactionType: uint16_t {
        Unknown = 0x0000,
        Transfer = 0x4154,
        Mosaic_Definition = 0x414D,
        Mosaic_Supply_Change = 0x424D,
        Mosaic_Levy_Change = 0x434D, // not defined yet
        Register_Namespace = 0x414E,
        Address_Alias = 0x424E,
        Mosaic_Alias = 0x434E,
        Modify_Multisig_Account = 0x4155,
        Aggregate_Complete = 0x4141,
        Aggregate_Bonded = 0x4241,
        Lock_Funds = 0x4148,
        Address_Property = 0x4150,
        Mosaic_Property = 0x4250,
        Transaction_Property = 0x4350,
        Secret_Lock = 0x4152,
        Secret_Proof = 0x4252,
		Storage_Payment = 0x4a62,
        Account_Link = 0x414C,
        Prepare_Bc_Drive = 0x4162,
        Data_Modification = 0x4262,
        Download = 0x4362,
        Download_Payment = 0x4962,
		Drive_Closure = 0x4e62,
        Data_Modification_Approval = 0x4462,
        Data_Modification_Cancel = 0x4562,
		Create_Liquidity_Provider = 0x4169,
		Manual_Rate_Change = 0x4269,
        Finish_Download = 0x4862,
        Replicator_Onboarding = 0x4662,
        Replicator_Offboarding = 0x4762,
        Deploy_Contract = 0x416E,
        Manual_Call = 0x426E,
        Automatic_Executions_Payment = 0x436E,
        Successful_End_Batch_Execution = 0x446E,
        Unsuccessful_End_Batch_Execution = 0x456E,
        End_Batch_Execution_Single = 0x466E,
        Synchronization_Single = 0x476E,
        Stream_Start = 0x4166,
        Stream_Finish = 0x4266,
        Stream_Payment = 0x4366
    };
	
	/// Returns transaction name by \a type.
	const char* GetTransactionName(TransactionType type);
	
	/// Return transaction version by \a type.
	uint8_t GetTransactionVersion(TransactionType type);
}
