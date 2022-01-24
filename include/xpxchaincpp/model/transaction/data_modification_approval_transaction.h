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
				uint64_t metaFilesSize,
				uint64_t usedDriveSize,
                uint8_t judgingKeysCount,
                uint8_t overlappingKeysCount,
                uint8_t judgedKeysCount,
                uint16_t opinionElementCount,
                const std::vector<Key>& publicKeys,
                const std::vector<Signature>& signatures,
                const std::vector<uint8_t>& presentOpinions,
                const std::vector<uint64_t>& opinions,
		        TArgs&&... args):
			TBase(TransactionType::Data_Modification_Approval, std::forward<TArgs>(args)...),
			driveKey_(driveKey),
			dataModificationId_(dataModificationId),
			fileStructureCdi_(fileStructureCdi),
			fileStructureSize_(fileStructureSize),
			metaFilesSize_(metaFilesSize),
			usedDriveSize_(usedDriveSize),
            judgingKeysCount_(judgingKeysCount),
            overlappingKeysCount_(overlappingKeysCount),
            judgedKeysCount_(judgedKeysCount),
            opinionElementCount_(opinionElementCount),
            publicKeys_(publicKeys),
            signatures_(signatures),
            presentOpinions_(presentOpinions),
            opinions_(opinions)
		{ }

		/// Returns drive key.
		const Key& driveKey() const;

		/// Returns identifier of the transaction that initiated the modification.
		const Hash256& dataModificationId() const;

		/// Returns Content Download Information for the File Structure.
		const Hash256& fileStructureCdi() const;

		/// Returns size of the File Structure.
		uint64_t fileStructureSize() const;

        /// Returns the size of metafiles including File Structure.
        uint64_t metaFilesSize() const;

		/// Returns total used disk space of the drive.
		uint64_t usedDriveSize() const;

        /// Returns number of replicators that provided their opinions, but on which no opinions were provided.
        uint8_t judgingKeysCount() const;

        /// Returns number of replicators that provided their opinions, but on which no opinions were provided.
        uint8_t overlappingKeysCount() const;

        /// Returns number of replicators that didn't provide their opinions, but on which at least one opinion was provided.
        uint8_t judgedKeysCount() const;

        /// Returns total number of opinion elements.
        uint8_t opinionElementCount() const;

        /// Returns replicators' public keys.
        std::vector<Key> publicKeys() const;

        /// Returns signatures of replicators' opinions.
        std::vector<Signature> signatures() const;

        /// Returns present opinions.
        /// Two-dimensional array of opinion element presence.
        /// Must be interpreted bitwise (1 if corresponding element exists, 0 otherwise).
        std::vector<uint8_t> presentOpinions() const;

        /// Returns total number of opinion elements.
        std::vector<uint64_t> opinions() const;

	private:
		Key driveKey_;
		Hash256 dataModificationId_;
		Hash256 fileStructureCdi_;
		uint64_t fileStructureSize_;
        uint64_t metaFilesSize_;
		uint64_t usedDriveSize_;
        uint8_t judgingKeysCount_;
        uint8_t overlappingKeysCount_;
        uint8_t judgedKeysCount_;
        uint16_t opinionElementCount_;
        std::vector<Key> publicKeys_;
        std::vector<Signature> signatures_;
        std::vector<uint8_t> presentOpinions_;
        std::vector<uint64_t> opinions_;
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
                                              uint64_t metaFilesSize,
                                              uint64_t usedDriveSize,
                                              uint8_t judgingKeysCount,
                                              uint8_t overlappingKeysCount,
                                              uint8_t judgedKeysCount,
                                              uint16_t opinionElementCount,
                                              const std::vector<Key>& publicKeys,
                                              const std::vector<Signature>& signatures,
                                              const std::vector<uint8_t>& presentOpinions,
                                              const std::vector<uint64_t>& opinions,
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
                                                      uint64_t metaFilesSize,
                                                      uint64_t usedDriveSize,
                                                      uint8_t judgingKeysCount,
                                                      uint8_t overlappingKeysCount,
                                                      uint8_t judgedKeysCount,
                                                      uint16_t opinionElementCount,
                                                      const std::vector<Key>& publicKeys,
                                                      const std::vector<Signature>& signatures,
                                                      const std::vector<uint8_t>& presentOpinions,
                                                      const std::vector<uint64_t>& opinions,
                                                      const Key& signer,
                                                      std::optional<NetworkIdentifier> networkId = std::nullopt);
}
