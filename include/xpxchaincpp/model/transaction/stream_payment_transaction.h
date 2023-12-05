/**
*** Copyright 2023 ProximaX Limited. All rights reserved.
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

    /// Stream payment transaction base class.
    template<typename TBase>
    class TStreamPaymentTransaction: public TBase {
    public:
        /// Creates stream payment transaction.
        template<typename... TArgs>
        explicit TStreamPaymentTransaction(const Key &driveKey,
                                           const Hash256 &streamId,
                                           const uint64_t additionalUploadSizeMegabytes,
                TArgs &&... args):
                TBase(TransactionType::Stream_Payment, std::forward<TArgs>(args)...),
                driveKey_(driveKey),
                streamId_(streamId),
                additionalUploadSizeMegabytes_(additionalUploadSizeMegabytes)
        { }

        /// Returns identifier of the drive.
        const Key& driveKey() const;

        /// Returns Id of stream to be finished.
        const Hash256& streamId() const;

        /// Returns actual size of stream.
        uint64_t additionalUploadSizeMegabytes() const;

    private:
        Key driveKey_;
        Hash256 streamId_;
        uint64_t additionalUploadSizeMegabytes_;
    };

    extern template class TStreamPaymentTransaction<Transaction>;
    extern template class TStreamPaymentTransaction<EmbeddedTransaction>;

    using StreamPaymentTransaction = TStreamPaymentTransaction<Transaction>;
    using EmbeddedStreamPaymentTransaction = TStreamPaymentTransaction<EmbeddedTransaction>;

    /// Creates stream payment transaction.
    /// \note Throws \c transaction_error if mosaics or message have invalid size.
    /// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
    std::unique_ptr<StreamPaymentTransaction>
    CreateStreamPaymentTransaction(const Key &driveKey,
                                   const Hash256 &streamId,
                                   const uint64_t additionalUploadSizeMegabytes,
                                   std::optional<Amount> maxFee = std::nullopt,
                                   std::optional<NetworkDuration> deadline = std::nullopt,
                                   std::optional<NetworkIdentifier> networkId = std::nullopt);


    /// Creates stream payment transaction.
    /// \note Throws \c transaction_error if mosaics or message have invalid size.
    /// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
    std::unique_ptr<EmbeddedStreamPaymentTransaction>
    CreateEmbeddedStreamPaymentTransaction(const Key &driveKey,
                                           const Hash256 &streamId,
                                           const uint64_t additionalUploadSizeMegabytes,
                                           const Key &signer,
                                           std::optional<NetworkIdentifier> networkId = std::nullopt);
}
