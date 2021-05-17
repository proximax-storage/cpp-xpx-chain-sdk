/**
*** Copyright 2021 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <xpxchaincpp/utils/buffer.h>
#include <xpxchaincpp/model/transaction/transaction.h>
#include <xpxchaincpp/model/transaction/embedded_transaction.h>

#include <memory>
#include <optional>
#include <string>
#include <utility>

namespace xpx_chain_sdk {

	/// Data modification approval transaction base class.
	template<typename TBase>
	class TDataModificationApprovalTransaction: public TBase {
	public:
		/// Creates data modification approval transaction.
		template<typename... TArgs>
		explicit TDataModificationApprovalTransaction(
				const Key& driveKey,
				const Hash256& dataModificationId,
				const Hash256& fileStructureCdi,
				uint64_t fileStructureSize,
				uint64_t usedDriveSize,
		        TArgs&&... args):
			TBase(TransactionType::Data_Modification_Approval, std::forward<TArgs>(args)...),
			driveKey_(driveKey),
			dataModificationId_(dataModificationId),
			fileStructureCdi_(fileStructureCdi),
			fileStructureSize_(fileStructureSize),
			usedDriveSize_(usedDriveSize)
		{ }

		/// Returns drive key.
		const Key& driveKey() const;

		/// Returns identifier of the transaction that initiated the modification.
		const Hash256& dataModificationId() const;

		/// Returns Content Download Information for the File Structure.
		const Hash256& fileStructureCdi() const;

		/// Returns size of the File Structure.
		uint64_t fileStructureSize() const;

		/// Returns total used disk space of the drive.
		uint64_t usedDriveSize() const;

	private:
		Key driveKey_;
		Hash256 dataModificationId_;
		Hash256 fileStructureCdi_;
		uint64_t fileStructureSize_;
		uint64_t usedDriveSize_;
	};

	extern template class TDataModificationApprovalTransaction<Transaction>;
	extern template class TDataModificationApprovalTransaction<EmbeddedTransaction>;

	using DataModificationApprovalTransaction = TDataModificationApprovalTransaction<Transaction>;
	using EmbeddedDataModificationApprovalTransaction = TDataModificationApprovalTransaction<EmbeddedTransaction>;

	/// Creates data modification approval transaction.
	/// \note Throws \c transaction_error if mosaics or message have invalid size.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<DataModificationApprovalTransaction>
	CreateDataModificationApprovalTransaction(const Key& driveKey,
	                          const Hash256& dataModificationId,
	                          const Hash256& fileStructureCdi,
	                          uint64_t fileStructureSize,
	                          uint64_t usedDriveSize,
	                          std::optional<Amount> maxFee = std::nullopt,
	                          std::optional<NetworkDuration> deadline = std::nullopt,
	                          std::optional<NetworkIdentifier> networkId = std::nullopt);


	/// Creates data modification approval transaction.
	/// \note Throws \c transaction_error if mosaics or message have invalid size.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<EmbeddedDataModificationApprovalTransaction>
	CreateEmbeddedDataModificationApprovalTransaction(const Key& driveKey,
	                                  const Hash256& dataModificationId,
	                                  const Hash256& fileStructureCdi,
	                                  uint64_t fileStructureSize,
	                                  uint64_t usedDriveSize,
	                                  const Key& signer,
	                                  std::optional<NetworkIdentifier> networkId = std::nullopt);
}
