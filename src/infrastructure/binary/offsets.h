
#pragma once

#include <cstddef>

namespace nem2_sdk { namespace internal { namespace binary {
	
	// Some offsets in transaction binary data.
	
	constexpr size_t Tx_Size_Start = 0;       // 4  bytes
	constexpr size_t Tx_Signature_Start = 4;  // 64 bytes
	constexpr size_t Tx_Signer_Start = 68;    // 32 bytes
	constexpr size_t Tx_Version_Start = 100;  // 2 bytes
	constexpr size_t Tx_Type_Start = 102;     // 2 bytes
	constexpr size_t Tx_Fee_Start = 104;      // 8 bytes
	constexpr size_t Tx_Deadline_Start = 112; // 8 bytes;
	constexpr size_t Tx_Signed_Data_Start = Tx_Version_Start;
	
	constexpr size_t Embedded_Tx_Size_Start = 0;     // 4  bytes
	constexpr size_t Embedded_Tx_Signer_Start = 4;   // 32 bytes
	constexpr size_t Embedded_Tx_Version_Start = 36; // 2 bytes
	constexpr size_t Embedded_Tx_Type_Start = 38;    // 2 bytes
	
	constexpr size_t Aggregate_Tx_Payload_Size_Start = 120; // 4 bytes
	constexpr size_t Aggregate_Tx_Payload_Start = 124;
}}}
