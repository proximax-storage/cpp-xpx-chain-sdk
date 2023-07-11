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
            case TransactionType::Storage_Payment:				return "storage payment";
            case TransactionType::Account_Link:					return "account link";
            case TransactionType::Prepare_Bc_Drive:				return "prepare bc drive";
            case TransactionType::Data_Modification:			return "data modification";
            case TransactionType::Download:						return "download";
            case TransactionType::Download_Payment:				return "download payment";
            case TransactionType::Drive_Closure:				return "drive closure";
            case TransactionType::Data_Modification_Approval:	return "data modification approval";
            case TransactionType::Data_Modification_Cancel:		return "data modification cancel";
            case TransactionType::Create_Liquidity_Provider:	return "create liquidity provider";
            case TransactionType::Manual_Rate_Change:		    return "manual rate change";
            case TransactionType::Finish_Download:      		return "finish download";
            case TransactionType::Replicator_Onboarding:		return "replicator onboarding";
            case TransactionType::Replicator_Offboarding:		return "replicator offboarding";
            case TransactionType::Deploy_Contract:                  return "Deploy Contract";
            case TransactionType::Manual_Call:                      return "Manual Call";
            case TransactionType::Automatic_Executions_Payment:     return "Automatic Executions Payment";
            case TransactionType::Successful_End_Batch_Execution:   return "Successful End Batch Execution";
            case TransactionType::Unsuccessful_End_Batch_Execution: return "Unsuccessful End Batch Execution";
            case TransactionType::End_Batch_Execution_Single:       return "End Batch Execution Single";
            case TransactionType::Synchronization_Single:           return "Synchronization Single";
            case TransactionType::Stream_Start:                     return "Stream Start";
            case TransactionType::Stream_Finish:                    return "Stream Finish";
            case TransactionType::Stream_Payment:                   return "Stream Payment";
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
            case TransactionType::Storage_Payment:				return 1;
            case TransactionType::Account_Link:					return 2;
            case TransactionType::Prepare_Bc_Drive:				return 1;
            case TransactionType::Data_Modification:			return 1;
            case TransactionType::Download:						return 1;
            case TransactionType::Download_Payment:				return 1;
            case TransactionType::Drive_Closure:				return 1;
            case TransactionType::Data_Modification_Approval:	return 1;
            case TransactionType::Data_Modification_Cancel:		return 1;
            case TransactionType::Create_Liquidity_Provider:	return 1;
            case TransactionType::Manual_Rate_Change:		    return 1;
            case TransactionType::Finish_Download:      		return 1;
            case TransactionType::Replicator_Onboarding:		return 1;
            case TransactionType::Replicator_Offboarding:		return 1;
            case TransactionType::Deploy_Contract:                  return 1;
            case TransactionType::Manual_Call:                      return 1;
            case TransactionType::Automatic_Executions_Payment:     return 1;
            case TransactionType::Successful_End_Batch_Execution:   return 1;
            case TransactionType::Unsuccessful_End_Batch_Execution: return 1;
            case TransactionType::End_Batch_Execution_Single:       return 1;
            case TransactionType::Synchronization_Single:           return 1;
            case TransactionType::Stream_Start:                     return 1;
            case TransactionType::Stream_Finish:                    return 1;
            case TransactionType::Stream_Payment:                   return 1;
            default: break;
        }

        return 0;
    }
}