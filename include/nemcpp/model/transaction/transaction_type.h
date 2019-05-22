
#pragma once

#include <cstdint>

namespace nem2_sdk {
	
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
		Account_Link = 0x414C
	};
	
	/// Returns transaction name by \a type.
	const char* GetTransactionName(TransactionType type);
	
	/// Return transaction version by \a type.
	uint8_t GetTransactionVersion(TransactionType type);
}
