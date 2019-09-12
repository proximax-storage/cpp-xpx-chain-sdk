
#pragma once

#include <xpxchaincpp/model/mosaic/mosaic_property.h>
#include <xpxchaincpp/model/transaction/transaction.h>
#include <xpxchaincpp/model/transaction/embedded_transaction.h>

#include <memory>
#include <optional>
#include <utility>

namespace xpx_chain_sdk {
	
	/// Mosaic definition transaction base class.
	template<typename TBase>
	class TMosaicDefinitionTransaction: public TBase {
	public:
		
		/// Creates mosaic definition transaction.
		template<typename... TArgs>
		explicit TMosaicDefinitionTransaction(uint32_t mosaicNonce,
		                                      MosaicId mosaicId,
		                                      MosaicFlags flags,
		                                      MosaicProperties mosaicProperties,
		                                      TArgs&&... args):
			TBase(TransactionType::Mosaic_Definition, std::forward<TArgs>(args)...),
			mosaicNonce_(mosaicNonce),
			mosaicId_(mosaicId),
			flags_(flags),
			mosaicProperties_(std::move(mosaicProperties))
		{ }
		
		/// Returns mosaic nonce.
		uint32_t mosaicNonce() const;
		
		/// Returns generated mosaic id.
		MosaicId mosaicId() const;

		/// Returns mosaic flags;
		MosaicFlags flags() const;
		
		/// Returns mosaic properties.
		const MosaicProperties& mosaicProperties() const;
		
	private:
		uint32_t mosaicNonce_;
		MosaicId mosaicId_;
		MosaicFlags flags_;
		MosaicProperties mosaicProperties_;
	};
	
	extern template class TMosaicDefinitionTransaction<Transaction>;
	extern template class TMosaicDefinitionTransaction<EmbeddedTransaction>;
	
	using MosaicDefinitionTransaction = TMosaicDefinitionTransaction<Transaction>;
	using EmbeddedMosaicDefinitionTransaction = TMosaicDefinitionTransaction<EmbeddedTransaction>;
	
	/// Creates mosaic definition transaction.
	/// \note Throws \c transaction_error if \a mosaicProperties has incorrect size.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<MosaicDefinitionTransaction>
	CreateMosaicDefinitionTransaction(uint32_t mosaicNonce,
	                                  MosaicId mosaicId,
	                                  MosaicFlags flags,
	                                  MosaicProperties mosaicProperties,
	                                  std::optional<Amount> maxFee = std::nullopt,
	                                  std::optional<NetworkDuration> deadline = std::nullopt,
	                                  std::optional<NetworkIdentifier> networkId = std::nullopt);
	
	/// Creates embedded mosaic definition transaction.
	/// \note Throws \c transaction_error if \a mosaicProperties has incorrect size.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<EmbeddedMosaicDefinitionTransaction>
	CreateEmbeddedMosaicDefinitionTransaction(uint32_t mosaicNonce,
	                                          MosaicId mosaicId,
	                                          MosaicFlags flags,
	                                          MosaicProperties mosaicProperties,
	                                          const Key& signer,
	                                          std::optional<NetworkIdentifier> networkId = std::nullopt);
}
