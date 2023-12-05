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

    /// Stream finish transaction base class.
    template<typename TBase>
    class TStreamFinishTransaction: public TBase {
    public:
        /// Creates stream finish transaction.
        template<typename... TArgs>
        explicit TStreamFinishTransaction(const Key& driveKey,
                                          const Hash256& streamId,
                                          const uint64_t actualUploadSizeMegabytes,
                                          const Hash256& streamStructureCdi,
                                          TArgs&&... args):
                TBase(TransactionType::Stream_Finish, std::forward<TArgs>(args)...),
                driveKey_(driveKey),
                streamId_(streamId),
                actualUploadSizeMegabytes_(actualUploadSizeMegabytes),
                streamStructureCdi_(streamStructureCdi)
        {}

        /// Returns key of drive.
        const Key& driveKey() const;

        /// Returns Id of stream to be finished.
        const Hash256& streamId() const;

        /// Returns expected size of stream.
        uint64_t actualUploadSizeMegabytes() const;

        /// Returns message of folderName string in bytes.
        const Hash256& streamStructureCdi() const;

    private:
        Key driveKey_;
        Hash256 streamId_;
        uint64_t actualUploadSizeMegabytes_;
        Hash256 streamStructureCdi_;
    };

    extern template class TStreamFinishTransaction<Transaction>;
    extern template class TStreamFinishTransaction<EmbeddedTransaction>;

    using StreamFinishTransaction = TStreamFinishTransaction<Transaction>;
    using EmbeddedStreamFinishTransaction = TStreamFinishTransaction<EmbeddedTransaction>;

    /// Creates stream finish transaction.
    /// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
    std::unique_ptr<StreamFinishTransaction>
    CreateStreamFinishTransaction(const Key& driveKey,
                                  const Hash256& streamId,
                                  const uint64_t actualUploadSizeMegabytes,
                                  const Hash256& streamStructureCdi,
                                  std::optional<Amount> maxFee = std::nullopt,
                                  std::optional<NetworkDuration> deadline = std::nullopt,
                                  std::optional<NetworkIdentifier> networkId = std::nullopt);

    /// Creates embedded stream finish transaction.
    /// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
    std::unique_ptr<EmbeddedStreamFinishTransaction>
    CreateEmbeddedStreamFinishTransaction(const Key& driveKey,
                                          const Hash256& streamId,
                                          const uint64_t actualUploadSizeMegabytes,
                                          const Hash256& streamStructureCdi,
                                          const Key &signer,
                                          std::optional<NetworkIdentifier> networkId = std::nullopt);
}
