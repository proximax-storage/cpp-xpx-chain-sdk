/**
*** Copyright 2023 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <xpxchaincpp/model/transaction/transaction.h>
#include <xpxchaincpp/model/transaction/embedded_transaction.h>

#include <memory>
#include <optional>
#include <utility>

namespace xpx_chain_sdk {

    /// Stream Start transaction base class.
    template<typename TBase>
    class TStreamStartTransaction: public TBase {
    public:
        /// Creates stream Start transaction.
        template<typename... TArgs>
        explicit TStreamStartTransaction(const Key &driveKey,
                                         const uint64_t expectedUploadSizeMegabytes,
                                         const uint16_t folderNameSize,
                                         const Amount &feedbackFeeAmount,
                                         const std::vector<uint8_t>& folderName,
                                         TArgs &&... args):
                TBase(TransactionType::Stream_Start, std::forward<TArgs>(args)...),
                driveKey_(driveKey),
                expectedUploadSizeMegabytes_(expectedUploadSizeMegabytes),
                folderName_(folderName),
                folderNameSize_(folderNameSize),
                feedbackFeeAmount_(feedbackFeeAmount)
        {}

        /// Returns key of drive.
        const Key& driveKey() const;

        /// Returns expected size of stream.
        uint64_t expectedUploadSizeMegabytes() const;

        /// Returns message of folderName string in bytes.
        uint16_t folderNameSize() const;

        /// Returns amount of XPXs to transfer to the drive.
        const Amount& feedbackFeeAmount() const;

        /// Returns folder name.
        std::vector<uint8_t> folderName() const;

    private:
        Key driveKey_;
        uint64_t expectedUploadSizeMegabytes_;
        uint16_t folderNameSize_;
        Amount feedbackFeeAmount_;
        std::vector<uint8_t> folderName_;
    };

    extern template class TStreamStartTransaction<Transaction>;
    extern template class TStreamStartTransaction<EmbeddedTransaction>;

    using StreamStartTransaction = TStreamStartTransaction<Transaction>;
    using EmbeddedStreamStartTransaction = TStreamStartTransaction<EmbeddedTransaction>;

    /// Creates stream Start transaction.
    /// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
    std::unique_ptr<StreamStartTransaction>
    CreateStreamStartTransaction(const Key &driveKey,
                                 const uint64_t expectedUploadSizeMegabytes,
                                 const uint16_t folderNameSize,
                                 const Amount &feedbackFeeAmount,
                                 const std::vector<uint8_t>& folderName,
                                 std::optional<Amount> maxFee = std::nullopt,
                                 std::optional<NetworkDuration> deadline = std::nullopt,
                                 std::optional<NetworkIdentifier> networkId = std::nullopt);

    /// Creates embedded stream Start transaction.
    /// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
    std::unique_ptr<EmbeddedStreamStartTransaction>
    CreateEmbeddedStreamStartTransaction(const Key &driveKey,
                                         const uint64_t expectedUploadSizeMegabytes,
                                         const uint16_t folderNameSize,
                                         const Amount &feedbackFeeAmount,
                                         const std::vector<uint8_t>& folderName,
                                         const Key &signer,
                                         std::optional<NetworkIdentifier> networkId = std::nullopt);
}
