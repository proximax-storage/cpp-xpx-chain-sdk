
#pragma once

#include <nemcpp/types.h>
#include <nemcpp/model/account/address.h>
#include <nemcpp/model/transaction/transaction_type.h>

namespace nem2_sdk {
	
	/// Account property type.
	enum class AccountPropertyType: uint8_t {
		Address = 0x01,
		Mosaic = 0x02,
		Transaction = 0x04
	};
	
	/// Account property rule.
	enum class AccountPropertyRule: uint8_t {
		Allow = 0x00,
		Block = 0x80
	};
	
	/// Account property.
	template<typename TValue, AccountPropertyType PropertyTypeId>
	struct AccountProperty {
		using ValueType = TValue;
		
		/// Returns property type.
		static constexpr AccountPropertyType GetType()
		{ return PropertyTypeId; }
		
		/// Property value.
		TValue value;
		
		/// Property rule.
		AccountPropertyRule rule;
	};
	
	/// Returns \c true if \a lhs and \a rhs are equal.
	template<typename TValue, AccountPropertyType PropertyTypeId>
	bool operator==(const AccountProperty<TValue, PropertyTypeId>& lhs, const AccountProperty<TValue, PropertyTypeId>& rhs)
	{
		return lhs.value == rhs.value && lhs.rule == rhs.rule;
	}
	
	/// Returns \c true if \a lhs and \a rhs are not equal.
	template<typename TValue, AccountPropertyType PropertyTypeId>
	bool operator!=(const AccountProperty<TValue, PropertyTypeId>& lhs, const AccountProperty<TValue, PropertyTypeId>& rhs)
	{
		return !(lhs == rhs);
	}
	
	/// Account address property.
	using AccountAddressProperty = AccountProperty<AddressData, AccountPropertyType::Address>;
	
	/// Account mosaic property.
	using AccountMosaicProperty = AccountProperty<MosaicId, AccountPropertyType::Mosaic>;
	
	/// Account transaction property.
	using AccountTransactionProperty = AccountProperty<TransactionType, AccountPropertyType::Transaction>;
}
