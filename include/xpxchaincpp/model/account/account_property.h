/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <xpxchaincpp/types.h>
#include <xpxchaincpp/model/account/address.h>
#include <xpxchaincpp/model/transaction/transaction_type.h>
#include <string>
#include <vector>

namespace xpx_chain_sdk {

	class Property {
	public:
		uint8_t propertyType;
		std::vector<std::string> values;
	};

	class AccountProperties {
	public:
		std::string address;
		std::vector<Property> properties;
	};

	/// Account property type.
	enum class AccountPropertyType : uint8_t {
		Address = 0x01,
		Mosaic = 0x02,
		Transaction = 0x04
	};

	/// Account property rule.
	enum class AccountPropertyRule : uint8_t {
		Allow = 0x00,
		Block = 0x80
	};
}

namespace xpx_chain_sdk {
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
