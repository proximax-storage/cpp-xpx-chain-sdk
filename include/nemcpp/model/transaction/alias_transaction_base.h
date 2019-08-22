
#pragma once

#include <nemcpp/model/transaction/alias_transaction_types.h>
#include <nemcpp/model/transaction/transaction.h>
#include <nemcpp/model/transaction/embedded_transaction.h>

#include <utility>

namespace xpx_sdk {
	
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
