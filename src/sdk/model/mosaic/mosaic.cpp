/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include <xpxchaincpp/model/mosaic/mosaic.h>
#include <xpxchaincpp/crypto/hash.h>

namespace xpx_chain_sdk {
	
	MosaicId Mosaic::GenerateId(const Key& owner, uint32_t nonce)
	{
		constexpr uint64_t namespaceFlag = 1ull << 63;
		auto hash = Sha3_256::Hash(nonce, owner);
		return reinterpret_cast<const MosaicId&>(*hash.data()) & ~namespaceFlag;
	}
	
	bool operator==(const Mosaic& lhs, const Mosaic& rhs)
	{
		return lhs.id == rhs.id && lhs.amount == rhs.amount;
	}
	
	bool operator!=(const Mosaic& lhs, const Mosaic& rhs)
	{
		return !(lhs == rhs);
	}
}
