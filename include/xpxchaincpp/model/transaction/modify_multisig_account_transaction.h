
#pragma once

#include <xpxchaincpp/model/transaction/modify_multisig_account_transaction_types.h>
#include <xpxchaincpp/model/transaction/transaction.h>
#include <xpxchaincpp/model/transaction/embedded_transaction.h>

#include <memory>
#include <optional>
#include <unordered_map>
#include <utility>

namespace xpx_chain_sdk {
	
	/// Multisig account modification transaction base class.
	template<typename TBase>
	class TModifyMultisigAccountTransaction: public TBase {
	public:
		/// Creates multisig account modification transaction.
		template<typename... TArgs>
		explicit TModifyMultisigAccountTransaction(int8_t minRemovalDelta,
		                                           int8_t minApprovalDelta,
		                                           const std::unordered_map<Key, CosignatoryModificationType>& cosignatoryModifications,
		                                           TArgs&&... args):
			TBase(TransactionType::Modify_Multisig_Account, std::forward<TArgs>(args)...),
			minRemovalDelta_(minRemovalDelta),
			minApprovalDelta_(minApprovalDelta)
		{
			for (const auto& [key, modificationType]: cosignatoryModifications) {
				cosignatoryModifications_[PublicAccount{ key, TBase::networkId() }] = modificationType;
			}
		}
		
		/// Returns relative change of minimum number of signatures needed to remove a cosignatory.
		int8_t minRemovalDelta() const;
		
		/// Returns relative change of minimum number of signatures required when approving a transaction.
		int8_t minApprovalDelta() const;
		
		/// Returns cosignatory modifications.
		const CosignatoryModifications& cosignatoryModifications() const;
		
	private:
		int8_t minRemovalDelta_;
		int8_t minApprovalDelta_;
		CosignatoryModifications cosignatoryModifications_;
	};
	
	extern template class TModifyMultisigAccountTransaction<Transaction>;
	extern template class TModifyMultisigAccountTransaction<EmbeddedTransaction>;
	
	using ModifyMultisigAccountTransaction = TModifyMultisigAccountTransaction<Transaction>;
	using EmbeddedModifyMultisigAccountTransaction = TModifyMultisigAccountTransaction<EmbeddedTransaction>;
	
	/// Creates multisig account modification transaction.
	/// \note Throws \c transaction_error if \a cosignatoriesModifications size is invalid.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<ModifyMultisigAccountTransaction>
	CreateModifyMultisigAccountTransaction(int8_t minRemovalDelta,
	                                       int8_t minApprovalDelta,
	                                       const std::unordered_map<Key, CosignatoryModificationType>& cosignatoryModifications,
	                                       std::optional<Amount> maxFee = std::nullopt,
	                                       std::optional<NetworkDuration> deadline = std::nullopt,
	                                       std::optional<NetworkIdentifier> networkId = std::nullopt);
	
	/// Creates embedded multisig account modification transaction.
	/// \note Throws \c transaction_error if \a cosignatoriesModifications size is invalid.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<EmbeddedModifyMultisigAccountTransaction>
	CreateEmbeddedModifyMultisigAccountTransaction(int8_t minRemovalDelta,
	                                               int8_t minApprovalDelta,
	                                               const std::unordered_map<Key, CosignatoryModificationType>& cosignatoryModifications,
	                                               const Key& signer,
	                                               std::optional<NetworkIdentifier> networkId = std::nullopt);
}
