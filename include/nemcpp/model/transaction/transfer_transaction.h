
#pragma once

#include <nemcpp/utils/buffer.h>
#include <nemcpp/model/account/address.h>
#include <nemcpp/model/mosaic/mosaic.h>
#include <nemcpp/model/transaction/transaction.h>
#include <nemcpp/model/transaction/embedded_transaction.h>

#include <memory>
#include <optional>
#include <string>
#include <utility>

namespace xpx_sdk {
	
	/// Constant to be used as empty message in transfer transaction.
	constexpr uint8_t Empty_Message[] = {0};
	
	/// Transfer transaction base class.
	template<typename TBase>
	class TTransferTransaction: public TBase {
	public:
		/// Creates transfer transaction.
		template<typename... TArgs>
		explicit TTransferTransaction(const Address& recipient,
		                              MosaicContainer mosaics,
		                              RawBuffer message,
		                              TArgs&&... args):
			TBase(TransactionType::Transfer, std::forward<TArgs>(args)...),
			recipient_(recipient),
			message_(message.begin(), message.end()),
			messageBuffer_(message_),
			mosaics_(std::move(mosaics))
		{ }
		
		/// Returns recipient.
		const Address& recipient() const;
		
		/// Returns message buffer.
		const RawBuffer& message() const;
		
		/// Returns message as a string.
		const std::string& messageStr() const;
		
		/// Returns mosaics.
		const MosaicContainer& mosaics() const;
		
		/// Returns amount of mosaic with specified \a mosaicId.
		Amount getAmount(MosaicId mosaicId) const;
		
	private:
		Address recipient_;
		std::string message_;
		RawBuffer messageBuffer_;
		MosaicContainer mosaics_;
	};
	
	extern template class TTransferTransaction<Transaction>;
	extern template class TTransferTransaction<EmbeddedTransaction>;
	
	using TransferTransaction = TTransferTransaction<Transaction>;
	using EmbeddedTransferTransaction = TTransferTransaction<EmbeddedTransaction>;
	
	/// Creates transfer transaction.
	/// \note Throws \c transaction_error if mosaics or message have invalid size.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<TransferTransaction>
	CreateTransferTransaction(const Address& recipient,
	                          MosaicContainer mosaics,
	                          RawBuffer message,
	                          std::optional<Amount> maxFee = std::nullopt,
	                          std::optional<NetworkDuration> deadline = std::nullopt,
	                          std::optional<NetworkIdentifier> networkId = std::nullopt);
	
	
	/// Creates transfer transaction.
	/// \note Throws \c transaction_error if mosaics or message have invalid size.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<EmbeddedTransferTransaction>
	CreateEmbeddedTransferTransaction(const Address& recipient,
	                                  MosaicContainer mosaics,
	                                  RawBuffer message,
	                                  const Key& signer,
	                                  std::optional<NetworkIdentifier> networkId = std::nullopt);
}
