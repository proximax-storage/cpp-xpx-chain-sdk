/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <xpxchaincpp/model/transaction/alias_transaction_types.h>
#include <xpxchaincpp/model/transaction/transaction.h>
#include <xpxchaincpp/model/transaction/embedded_transaction.h>

#include <utility>

namespace xpx_chain_sdk {
	
	/// Base class for alias transactions.
	template<typename TBase>
	class TAliasTransactionBase: public TBase {
	public:
		/// Creates base alias transaction.
		template<typename... TArgs>
		explicit TAliasTransactionBase(AliasTransactionAction action,
		                               NamespaceId aliasNamespaceId,
		                               TArgs&&... args):
			TBase(std::forward<TArgs>(args)...),
			action_(action),
			aliasNamespaceId_(aliasNamespaceId)
		{ }
		
		/// Returns alias transaction action.
		AliasTransactionAction action() const;
		
		/// Returns id of alias namespace.
		NamespaceId aliasNamespaceId() const;
		
	private:
		AliasTransactionAction action_;
		NamespaceId aliasNamespaceId_;
	};
	
	extern template class TAliasTransactionBase<Transaction>;
	extern template class TAliasTransactionBase<EmbeddedTransaction>;
}
