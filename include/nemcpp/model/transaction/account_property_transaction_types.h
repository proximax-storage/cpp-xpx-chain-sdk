
#pragma once

#include <nemcpp/types.h>
#include <nemcpp/utils/comparator.h>
#include <nemcpp/model/transaction/transaction_type.h>

#include <cstdint>
#include <set>

namespace xpx_sdk {
	
	/// Account property modification type.
	enum class AccountPropertyModificationType: uint8_t {
		Add,
		Del
	};
	
	/// Account property modification type.
	template<typename TValue>
	struct AccountPropertyModification {
		TValue value;
		AccountPropertyModificationType modificationType;
	};
	
	/// Account property modifications container.
	template<typename TValue>
	using AccountPropertyModifications =
		std::set<AccountPropertyModification<TValue>,
		         data_comparator<AccountPropertyModification<TValue>,
		                         TValue,
		                         &AccountPropertyModification<TValue>::value>>;
	
	/// Account address property modifications.
	using AccountAddressPropertyModifications = AccountPropertyModifications<AddressData>;
	
	/// Account mosaic property modifications.
	using AccountMosaicPropertyModifications = AccountPropertyModifications<MosaicId>;
	
	/// Account transaction property modifications.
	using AccountTransactionPropertyModifications = AccountPropertyModifications<TransactionType>;
}
