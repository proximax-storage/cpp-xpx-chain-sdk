/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include <xpxchaincpp/model/transaction/account_property_transaction.h>

namespace xpx_chain_sdk {
	
	template<typename TBase, typename TProperty>
	AccountPropertyRule TAccountPropertyTransaction<TBase, TProperty>::propertyRule() const
	{
		return propertyRule_;
	}

	template<typename TBase, typename TProperty>
	const typename TAccountPropertyTransaction<TBase, TProperty>::PropertyModifications&
	TAccountPropertyTransaction<TBase, TProperty>::propertyModifications() const
	{
		return propertyModifications_;
	}

	template<typename TBase, typename TProperty>
	TransactionType TAccountPropertyTransaction<TBase, TProperty>::getTransactionType()
	{
		switch (TProperty::GetType()) {
		case AccountPropertyType::Address:
			return TransactionType::Address_Property;
		case AccountPropertyType::Mosaic:
			return TransactionType::Mosaic_Property;
		case AccountPropertyType::Transaction:
			return TransactionType::Transaction_Property;
		default:
			XPX_CHAIN_SDK_THROW_1(transaction_error, "unknown property for account property transaction",
			                 to_underlying_type(TProperty::GetType()));
		}
	}

	template class TAccountPropertyTransaction<Transaction, AccountAddressProperty>;
	template class TAccountPropertyTransaction<EmbeddedTransaction, AccountAddressProperty>;
	template class TAccountPropertyTransaction<Transaction, AccountMosaicProperty>;
	template class TAccountPropertyTransaction<EmbeddedTransaction, AccountMosaicProperty>;
	template class TAccountPropertyTransaction<Transaction, AccountTransactionProperty>;
	template class TAccountPropertyTransaction<EmbeddedTransaction, AccountTransactionProperty>;
}
