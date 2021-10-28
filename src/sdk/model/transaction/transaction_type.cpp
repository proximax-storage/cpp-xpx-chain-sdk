/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include <xpxchaincpp/model/transaction/transaction_type.h>

namespace xpx_chain_sdk {
	
	const char* GetTransactionName(TransactionType type)
	{
		switch (type) {
		case TransactionType::Transfer:						return "transfer";
		case TransactionType::Mosaic_Definition:			return "mosaic definition";
		case TransactionType::Mosaic_Supply_Change:			return "mosaic supply change";
		case TransactionType::Mosaic_Levy_Change:			return "mosaic levy change";
		case TransactionType::Register_Namespace:			return "register namespace";
		case TransactionType::Address_Alias:				return "address alias";
		case TransactionType::Mosaic_Alias:					return "mosaic alias";
		case TransactionType::Modify_Multisig_Account:		return "modify multisig account";
		case TransactionType::Aggregate_Complete:			return "aggregate complete";
		case TransactionType::Aggregate_Bonded:				return "aggregate bonded";
		case TransactionType::Lock_Funds:					return "lock funds";
		case TransactionType::Address_Property:				return "address property";
		case TransactionType::Mosaic_Property:				return "mosaic property";
		case TransactionType::Transaction_Property:			return "transaction property";
		case TransactionType::Secret_Lock:					return "secret lock";
		case TransactionType::Secret_Proof:					return "secret proof";
		case TransactionType::Account_Link:					return "account link";
		case TransactionType::Prepare_Bc_Drive:				return "prepare bc drive";
		case TransactionType::Data_Modification:			return "data modification";
		case TransactionType::Download:						return "download";
		case TransactionType::Data_Modification_Approval:	return "data modification approval";
		case TransactionType::Data_Modification_Cancel:		return "data modification cancel";
		case TransactionType::Replicator_Onboarding:		return "replicator onboarding";
		case TransactionType::Replicator_Offboarding:		return "replicator offboarding";
		default: break;
		}
		
		return "unknown";
	}
	
	uint8_t GetTransactionVersion(TransactionType type)
	{
		switch (type) {
		case TransactionType::Transfer:						return 3;
		case TransactionType::Mosaic_Definition:			return 3;
		case TransactionType::Mosaic_Supply_Change:			return 2;
		case TransactionType::Mosaic_Levy_Change:			return 0;
		case TransactionType::Register_Namespace:			return 2;
		case TransactionType::Address_Alias:				return 1;
		case TransactionType::Mosaic_Alias:					return 1;
		case TransactionType::Modify_Multisig_Account:		return 3;
		case TransactionType::Aggregate_Complete:			return 2;
		case TransactionType::Aggregate_Bonded:				return 2;
		case TransactionType::Lock_Funds:					return 1;
		case TransactionType::Address_Property:				return 1;
		case TransactionType::Mosaic_Property:				return 1;
		case TransactionType::Transaction_Property:			return 1;
		case TransactionType::Secret_Lock:					return 1;
		case TransactionType::Secret_Proof:					return 1;
		case TransactionType::Account_Link:					return 2;
		case TransactionType::Prepare_Bc_Drive:				return 1;
		case TransactionType::Data_Modification:			return 1;
		case TransactionType::Download:						return 1;
		case TransactionType::Data_Modification_Approval:	return 1;
		case TransactionType::Data_Modification_Cancel:		return 1;
		case TransactionType::Replicator_Onboarding:		return 1;
		case TransactionType::Replicator_Offboarding:		return 1;
		default: break;
		}
		
		return 0;
	}
}