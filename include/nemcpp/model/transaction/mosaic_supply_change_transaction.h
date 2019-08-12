
#pragma once

#include <nemcpp/model/transaction/mosaic_supply_change_transaction_types.h>
#include <nemcpp/model/transaction/transaction.h>
#include <nemcpp/model/transaction/embedded_transaction.h>

#include <memory>
#include <optional>
#include <utility>

namespace xpx_sdk::complex_transactions {
	
	/// Mosaic supply change transaction base class.
	template<typename TBase>
	class TMosaicSupplyChangeTransaction: public TBase {
	public:
		/// Creates mosaic supply change transaction.
		template<typename... TArgs>
		explicit TMosaicSupplyChangeTransaction(MosaicId mosaicId,
		                                        MosaicSupplyChangeDirection changeDirection,
		                                        Amount changeDelta,
		                                        TArgs&&... args):
			TBase(TransactionType::Mosaic_Supply_Change, std::forward<TArgs>(args)...),
			mosaicId_(mosaicId),
			changeDirection_(changeDirection),
			changeDelta_(changeDelta)
		{ }
		
		/// Returns mosaic id.
		MosaicId mosaicId() const;
		
		/// Returns mosaic supply change direction.
		MosaicSupplyChangeDirection changeDirection() const;
		
		/// Returns mosaic supply change delta.
		Amount changeDelta() const;
		
	private:
		MosaicId mosaicId_;
		MosaicSupplyChangeDirection changeDirection_;
		Amount changeDelta_;
	};
	
	extern template class TMosaicSupplyChangeTransaction<Transaction>;
	extern template class TMosaicSupplyChangeTransaction<EmbeddedTransaction>;
	
	using MosaicSupplyChangeTransaction = TMosaicSupplyChangeTransaction<Transaction>;
	using EmbeddedMosaicSupplyChangeTransaction = TMosaicSupplyChangeTransaction<EmbeddedTransaction>;
	
	/// Creates mosaic supply change transaction.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<MosaicSupplyChangeTransaction>
	CreateMosaicSupplyChangeTransaction(MosaicId mosaicId,
	                                    MosaicSupplyChangeDirection changeDirection,
	                                    Amount changeDelta,
	                                    std::optional<Amount> maxFee = std::nullopt,
	                                    std::optional<NetworkDuration> deadline = std::nullopt,
	                                    std::optional<NetworkIdentifier> networkId = std::nullopt);
	
	/// Creates embedded mosaic supply change transaction.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<EmbeddedMosaicSupplyChangeTransaction>
	CreateEmbeddedMosaicSupplyChangeTransaction(MosaicId mosaicId,
	                                            MosaicSupplyChangeDirection changeDirection,
	                                            Amount changeDelta,
	                                            const Key& signer,
	                                            std::optional<NetworkIdentifier> networkId = std::nullopt);
}
