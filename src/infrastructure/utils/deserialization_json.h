/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <infrastructure/json/parser.h>
#include <infrastructure/json/dto/block_dto.h>
#include <infrastructure/json/dto/drives_page_dto.h>
#include <infrastructure/json/dto/drive_state_notification_dto.h>
#include <infrastructure/json/dto/download_channels_page_dto.h>
#include <infrastructure/json/dto/replicators_page_dto.h>
#include <infrastructure/json/dto/mosaic_dto.h>
#include <infrastructure/json/dto/namespace_dto.h>
#include <infrastructure/json/dto/height.h>
#include <infrastructure/json/dto/account_info_dto.h>
#include <infrastructure/json/dto/account_names_dto.h>
#include <infrastructure/json/dto/multiple_namespace_dto.h>
#include <infrastructure/json/dto/multiple_blocks_dto.h>
#include <infrastructure/json/dto/multisig_graph_dto.h>
#include <infrastructure/json/dto/network_config_dto.h>
#include <infrastructure/json/dto/network_info_dto.h>
#include <infrastructure/json/dto/network_version_dto.h>
#include <infrastructure/json/dto/signer_info_notification_dto.h>
#include <infrastructure/json/dto/transaction_dto.h>
#include <infrastructure/json/dto/transactions_page_dto.h>
#include <infrastructure/json/dto/transaction_notification_dto.h>
#include <infrastructure/json/dto/transaction_status_notification_dto.h>
#include <infrastructure/json/dto/uid_dto.h>
#include <infrastructure/json/dto/liquidity_provider_dto.h>
#include <infrastructure/json/dto/liquidity_providers_page_dto.h>
#include <infrastructure/json/dto/meta_dto.h>
#include <infrastructure/json/dto/storage_dto.h>
#include <infrastructure/json/dto/error_message_dto.h>
#include <infrastructure/json/dto/supercontract_v2_dto.h>

#include <xpxchaincpp/model/blockchain/block.h>
#include <xpxchaincpp/model/blockchain/score.h>
#include <xpxchaincpp/model/blockchain/height.h>
#include <xpxchaincpp/model/blockchain/storage_info.h>
#include <xpxchaincpp/model/blockchain/multiple_block.h>
#include <xpxchaincpp/model/mosaic/multiple_mosaic.h>
#include <xpxchaincpp/model/mosaic/mosaic.h>
#include <xpxchaincpp/model/account/account_info.h>
#include <xpxchaincpp/model/account/account_names.h>
#include <xpxchaincpp/model/mosaic/mosaic_name.h>
#include <xpxchaincpp/model/mosaic/mosaic_names.h>
#include <xpxchaincpp/model/namespace/namespace_info.h>
#include <xpxchaincpp/model/namespace/namespace_name.h>
#include <xpxchaincpp/model/namespace/namespace_names.h>
#include <xpxchaincpp/model/namespace/multiple_namespace_info.h>
#include <xpxchaincpp/model/account/account_properties.h>
#include <xpxchaincpp/model/network/network_config.h>
#include <xpxchaincpp/model/network/network_info.h>
#include <xpxchaincpp/model/network/network_version.h>
#include <xpxchaincpp/model/supercontract_v2/supercontract.h>
#include <xpxchaincpp/model/transaction_simple/transaction.h>
#include <xpxchaincpp/model/transaction_simple/transaction_status.h>
#include <xpxchaincpp/model/transaction_simple/transaction_info.h>
#include <xpxchaincpp/model/transaction_simple/transactions_page.h>
#include <xpxchaincpp/model/notification/drive_state_notification.h>
#include <xpxchaincpp/model/notification/signer_info_notification.h>
#include <xpxchaincpp/model/notification/transaction_notification.h>
#include <xpxchaincpp/model/notification/transaction_status_notification.h>
#include <xpxchaincpp/model/storage/drives_page.h>
#include <xpxchaincpp/model/storage/download_channels_page.h>
#include <xpxchaincpp/model/storage/replicators_page.h>
#include <xpxchaincpp/model/liquidity_provider/liquidity_providers_page.h>
#include <sdk/model/notification/websocket_uid.h>
#include <sdk/model/notification/websocket_meta.h>



#include <xpxchaincpp/exceptions.h>
#include <xpxchaincpp/model/account/multiple_account_info.h>
#include <xpxchaincpp/model/transaction_simple/transaction_container.h>
#include "xpxchaincpp/client.h"


using namespace xpx_chain_sdk::transactions_info;

namespace xpx_chain_sdk { namespace  internal { namespace json {
    namespace dto {
        template<typename Object, typename ObjectDto>
        Object fromDto(const ObjectDto& dto);

        template<typename Object, typename ObjectDto>
        Object from_json(const std::string &jsonStr) {
            ObjectDto dto;

            auto result = Parser::Read(dto, jsonStr);
            if (!result) {
                XPX_CHAIN_SDK_THROW_1(serialization_error, "Cannot parse JSON. Error with:", result.invalidField())
            }

            return fromDto<Object, ObjectDto>(dto);
        }

		AggregateTransaction aggregateTransactionFromJson(const std::string& jsonStr);

		TransactionContainer transactions_from_json(const std::string& jsonStr);

		std::shared_ptr<transactions_info::BasicTransaction> transaction_from_json(const std::string& jsonStr);

        template<>
		TransactionContainer fromDto<TransactionContainer, TransactionContainerDto>(const TransactionContainerDto& dto);

        template<>
        BlockMeta fromDto<BlockMeta, BlockMetaDto>(const BlockMetaDto &dto);

        template<>
        BlockData fromDto<BlockData, BlockDataDto>(const BlockDataDto &dto);

        template<>
        Block fromDto<Block, BlockDto>(const BlockDto &dto);

        template<>
        StorageInfo fromDto<StorageInfo, StorageInfoDto>(const StorageInfoDto &dto) ;

        template<>
        ScoreInfo fromDto<ScoreInfo, ScoreInfoDto>(const ScoreInfoDto &dto) ;

        template<>
        Height fromDto<Height, HeightDto>(const HeightDto& dto) ;

        template<>
        Mosaic fromDto<Mosaic, MosaicDto>(const MosaicDto& dto);

        template<>
        MosaicMeta fromDto<MosaicMeta, MosaicMetaDto>(const MosaicMetaDto& dto);

        template<>
        MosaicData fromDto<MosaicData, MosaicDataDto>(const MosaicDataDto& dto) ;

        template<>
        MosaicInfo fromDto<MosaicInfo, MosaicInfoDto>(const MosaicInfoDto &dto);

        template<>
        MultipleMosaicInfo fromDto<MultipleMosaicInfo, MultipleMosaicInfoDto>(const MultipleMosaicInfoDto& dto);

        template<>
        MosaicName fromDto<MosaicName, MosaicNameDto>(const MosaicNameDto& dto);

        template<>
        MosaicNames fromDto<MosaicNames, MosaicNamesDto>(const MosaicNamesDto& dto);

        template<>
        Alias fromDto<Alias, AliasDto>(const AliasDto& dto);

        template<>
        NamespaceMeta fromDto<NamespaceMeta, NamespaceMetaDto>(const NamespaceMetaDto& dto);

        template<>
        NamespaceData fromDto<NamespaceData, NamespaceDataDto>(const NamespaceDataDto& dto);

        template<>
        NamespaceInfo fromDto<NamespaceInfo, NamespaceInfoDto>(const NamespaceInfoDto& dto);

        template<>
        MultipleNamespaceInfo fromDto<MultipleNamespaceInfo, MultipleNamespaceDto>(const MultipleNamespaceDto& dto);

        template<>
        NamespaceName fromDto<NamespaceName, NamespaceNameDto>(const NamespaceNameDto& dto);

        template<>
        NamespaceNames fromDto<NamespaceNames, NamespaceNamesDto>(const NamespaceNamesDto& dto);

        template<>
        MultipleBlock fromDto<MultipleBlock, MultipleBlocksDto>(const MultipleBlocksDto& dto);

        template<>
        Property fromDto<Property, PropertyDto>(const PropertyDto& dto);

        template<>
        AccountProperties fromDto<AccountProperties, AccountPropertyDto>(const AccountPropertyDto& dto) ;

        template<>
        MultipleAccountProperty fromDto<MultipleAccountProperty, MultipleAccountPropertyDto>(const MultipleAccountPropertyDto& dto);

        template<>
        MultisigInfo fromDto<MultisigInfo, MultisigInfoDto> (const MultisigInfoDto& dto);

        template<>
        AccountInfo fromDto<AccountInfo, AccountInfoDto> (const AccountInfoDto& dto);

        template<>
        MultipleAccountInfo fromDto<MultipleAccountInfo, MultipleAccountInfoDto>(const MultipleAccountInfoDto & dto);

        template<>
        MultisigLevel fromDto<MultisigLevel, MultisigLevelDto>(const MultisigLevelDto& dto);

        template<>
        MultisigGraph fromDto<MultisigGraph, MultisigGraphDto>(const MultisigGraphDto& dto);

        template<>
        NetworkInfo fromDto<NetworkInfo, NetworkInfoDto> (const NetworkInfoDto& dto);

        template<>
        DriveData fromDto<DriveData, DriveDataDto>(const DriveDataDto &dto);

        template<>
        Drive fromDto<Drive, DriveDto>(const DriveDto &dto);

        template<>
        DriveInfo fromDto<DriveInfo, DriveInfoDto>(const DriveInfoDto &dto);

        template<>
        xpx_chain_sdk::drives_page::Pagination fromDto<xpx_chain_sdk::drives_page::Pagination, dto::drives_page::PaginationDto>(const dto::drives_page::PaginationDto &dto);

        template<>
        xpx_chain_sdk::drives_page::DrivesPage fromDto<xpx_chain_sdk::drives_page::DrivesPage, drives_page::DrivesPageDto>(const drives_page::DrivesPageDto &dto);

        template<>
        ActiveDataModification fromDto<ActiveDataModification, ActiveDataModificationDto>(const ActiveDataModificationDto &dto);

        template<>
        CompletedDataModification fromDto<CompletedDataModification, CompletedDataModificationDto>(const CompletedDataModificationDto &dto);

        template<>
        ConfirmedUsedSize fromDto<ConfirmedUsedSize, ConfirmedUsedSizeDto>(const ConfirmedUsedSizeDto &dto);

        template<>
        Shard fromDto<Shard, ShardDto>(const ShardDto &dto);

        template<>
        Verification fromDto<Verification, VerificationDto>(const VerificationDto &dto);

        template<>
        DownloadShard fromDto<DownloadShard, DownloadShardDto>(const DownloadShardDto &dto);

        template<>
        DataModificationShard fromDto<DataModificationShard, DataModificationShardDto>(const DataModificationShardDto &dto);

        template<>
        Replicator fromDto<Replicator, ReplicatorDto>(const ReplicatorDto &dto);

        template<>
        ReplicatorData fromDto<ReplicatorData, ReplicatorDataDto>(const ReplicatorDataDto &dto);

        template<>
        UploadInfo fromDto<UploadInfo, UploadInfoDto>(const UploadInfoDto &dto);

        template<>
        xpx_chain_sdk::replicators_page::Pagination fromDto<xpx_chain_sdk::replicators_page::Pagination, dto::replicators_page::PaginationDto>(const dto::replicators_page::PaginationDto &dto);

        template<>
        xpx_chain_sdk::replicators_page::ReplicatorsPage fromDto<xpx_chain_sdk::replicators_page::ReplicatorsPage, replicators_page::ReplicatorsPageDto>(const replicators_page::ReplicatorsPageDto &dto);

        template<>
        DownloadChannel fromDto<DownloadChannel, DownloadChannelDto>(const DownloadChannelDto &dto);

        template<>
        DownloadChannelData fromDto<DownloadChannelData, DownloadChannelDataDto>(const DownloadChannelDataDto &dto);

        template<>
        CumulativePayment fromDto<CumulativePayment, CumulativePaymentDto>(const CumulativePaymentDto &dto);

        template<>
        xpx_chain_sdk::download_channels_page::Pagination fromDto<xpx_chain_sdk::download_channels_page::Pagination, dto::download_channels_page::PaginationDto>(const dto::download_channels_page::PaginationDto &dto);

        template<>
        xpx_chain_sdk::download_channels_page::DownloadChannelsPage fromDto<xpx_chain_sdk::download_channels_page::DownloadChannelsPage, download_channels_page::DownloadChannelsPageDto>(const download_channels_page::DownloadChannelsPageDto &dto);

        template<>
        RateData fromDto<RateData, RateDataDto>(const RateDataDto &dto);

        template<>
        TurnoverData fromDto<TurnoverData, TurnoverDataDto>(const TurnoverDataDto &dto);

        template<>
        LiquidityProviderData fromDto<LiquidityProviderData, LiquidityProviderDataDto>(const LiquidityProviderDataDto &dto);

        template<>
        LiquidityProvider fromDto<LiquidityProvider, LiquidityProviderDto>(const LiquidityProviderDto &dto);

        template<>
        MultipleLiquidityProviders fromDto<MultipleLiquidityProviders , MultipleLiquidityProvidersDto>(const MultipleLiquidityProvidersDto &dto);

        template<>
        xpx_chain_sdk::liquidity_providers_page::Pagination fromDto<xpx_chain_sdk::liquidity_providers_page::Pagination, dto::liquidity_providers_page::PaginationDto>(const dto::liquidity_providers_page::PaginationDto &dto);

        template<>
        xpx_chain_sdk::liquidity_providers_page::LiquidityProvidersPage fromDto<xpx_chain_sdk::liquidity_providers_page::LiquidityProvidersPage, liquidity_providers_page::LiquidityProvidersPageDto>(const liquidity_providers_page::LiquidityProvidersPageDto &dto);

        /// Transaction Meta

        template<>
		TransactionInfo fromDto<TransactionInfo, TransactionInfoDto>(const TransactionInfoDto& dto);

		template<>
		TransactionStatus fromDto<TransactionStatus, TransactionStatusDto>(const TransactionStatusDto& dto);

		template<>
		MultipleTransactionInfo fromDto<MultipleTransactionInfo, MultipleTransactionInfoDto>(const MultipleTransactionInfoDto& dto);

		template<>
		MultipleTransactionStatus fromDto<MultipleTransactionStatus, MultipleTransactionStatusDto>(const MultipleTransactionStatusDto& dto);

        /// Transactions
        template<>
        xpx_chain_sdk::transactions_page::TransactionsPage fromDto<xpx_chain_sdk::transactions_page::TransactionsPage, dto::transactions_page::TransactionsPageDto>(const dto::transactions_page::TransactionsPageDto& dto);

        template<>
        xpx_chain_sdk::transactions_page::Pagination fromDto<xpx_chain_sdk::transactions_page::Pagination, dto::transactions_page::PaginationDto>(const dto::transactions_page::PaginationDto &dto);

        template<>
		Cosignature fromDto<Cosignature, CosignatureDto>(const CosignatureDto& dto);

		template<>
		CosignatoryModification fromDto<CosignatoryModification, CosignatoryModificationDto>(const CosignatoryModificationDto& dto);

		template<>
		AccountTransactionPropertyModification fromDto<AccountTransactionPropertyModification, TransactionPropertyModificationDto>(const TransactionPropertyModificationDto& dto);

		template<>
		AccountMosaicPropertyModification fromDto<AccountMosaicPropertyModification, MosaicPropertyModificationDto>(const MosaicPropertyModificationDto& dto);

		template<>
		AccountAddressPropertyModification fromDto<AccountAddressPropertyModification, AddressPropertyModificationDto>(const AddressPropertyModificationDto& dto);

		template<>
		Transaction fromDto<Transaction, TransactionDto >(const TransactionDto & dto);

		template<>
		transactions_info::EmbeddedTransaction fromDto<transactions_info::EmbeddedTransaction, EmbeddedTransactionDto >(const EmbeddedTransactionDto & dto);

		template<>
		AggregateTransaction fromDto<AggregateTransaction, AggregateTransactionDto >(const AggregateTransactionDto & dto);

		template<>
		AccountLinkTransaction fromDto<AccountLinkTransaction, AccountLinkTransactionDto >(const AccountLinkTransactionDto & dto);

		template<>
		LockFundsTransaction fromDto<LockFundsTransaction, LockFundsTransactionDto >(const LockFundsTransactionDto & dto);

		template<>
		ModifyMultisigAccountTransaction fromDto<ModifyMultisigAccountTransaction, ModifyMultisigAccountTransactionDto >(const ModifyMultisigAccountTransactionDto & dto);

		template<>
		MosaicDefinitionTransaction fromDto<MosaicDefinitionTransaction, MosaicDefinitionTransactionDto >(const MosaicDefinitionTransactionDto & dto);

		template<>
		MosaicSupplyChangeTransaction fromDto<MosaicSupplyChangeTransaction, MosaicSupplyChangeTransactionDto >(const MosaicSupplyChangeTransactionDto & dto);


		template<>
		RegisterNamespaceTransaction fromDto<RegisterNamespaceTransaction, RegisterNamespaceTransactionDto >(const RegisterNamespaceTransactionDto & dto);

		template<>
		SecretLockTransaction fromDto<SecretLockTransaction, SecretLockTransactionDto >(const SecretLockTransactionDto & dto);

		template<>
		SecretProofTransaction fromDto<SecretProofTransaction, SecretProofTransactionDto >(const SecretProofTransactionDto & dto);

        template<>
        StoragePaymentTransaction fromDto<StoragePaymentTransaction, StoragePaymentTransactionDto >(const StoragePaymentTransactionDto & dto);

		template<>
		TransferTransaction fromDto<TransferTransaction, TransferTransactionDto >(const TransferTransactionDto & dto);

		template<>
		AliasTransactionBase fromDto<AliasTransactionBase, AliasTransactionBaseDto >(const AliasTransactionBaseDto & dto);

		template<>
		AddressAliasTransaction fromDto<AddressAliasTransaction, AddressAliasTransactionDto >(const AddressAliasTransactionDto & dto);

		template<>
		MosaicAliasTransaction fromDto<MosaicAliasTransaction, MosaicAliasTransactionDto >(const MosaicAliasTransactionDto & dto);

		template<>
		PrepareBcDriveTransaction fromDto<PrepareBcDriveTransaction, PrepareBcDriveTransactionDto >(const PrepareBcDriveTransactionDto & dto);

        template<>
        CreateLiquidityProviderTransaction fromDto<CreateLiquidityProviderTransaction, CreateLiquidityProviderTransactionDto >(const CreateLiquidityProviderTransactionDto & dto);

        template<>
        ManualRateChangeTransaction fromDto<ManualRateChangeTransaction, ManualRateChangeTransactionDto >(const ManualRateChangeTransactionDto & dto);

		template<>
		DataModificationTransaction fromDto<DataModificationTransaction, DataModificationTransactionDto >(const DataModificationTransactionDto & dto);

		template<>
		DownloadTransaction fromDto<DownloadTransaction, DownloadTransactionDto >(const DownloadTransactionDto & dto);

        template<>
        DownloadPaymentTransaction fromDto<DownloadPaymentTransaction, DownloadPaymentTransactionDto >(const DownloadPaymentTransactionDto & dto);

        template<>
        DriveClosureTransaction fromDto<DriveClosureTransaction, DriveClosureTransactionDto >(const DriveClosureTransactionDto & dto);

		template<>
		DataModificationApprovalTransaction fromDto<DataModificationApprovalTransaction, DataModificationApprovalTransactionDto >(const DataModificationApprovalTransactionDto & dto);

		template<>
		DataModificationCancelTransaction fromDto<DataModificationCancelTransaction, DataModificationCancelTransactionDto >(const DataModificationCancelTransactionDto & dto);

        template<>
        FinishDownloadTransaction fromDto<FinishDownloadTransaction, FinishDownloadTransactionDto >(const FinishDownloadTransactionDto & dto);

        template<>
		ReplicatorOnboardingTransaction fromDto<ReplicatorOnboardingTransaction, ReplicatorOnboardingTransactionDto >(const ReplicatorOnboardingTransactionDto & dto);

        template<>
        ReplicatorOffboardingTransaction fromDto<ReplicatorOffboardingTransaction, ReplicatorOffboardingTransactionDto >(const ReplicatorOffboardingTransactionDto & dto);

        /// Utils
        template<>
        ErrorMessage fromDto<ErrorMessage, ErrorMessageDto >(const ErrorMessageDto & dto);

        /// Supercontract V2

        template<>
        DeployContractTransaction fromDto<DeployContractTransaction, DeployContractTransactionDto>(const DeployContractTransactionDto& dto);

        template<>
        ManualCallTransaction fromDto<ManualCallTransaction, ManualCallTransactionDto>(const ManualCallTransactionDto& dto);

        template<>
        AutomaticExecutionsPaymentTransaction fromDto<AutomaticExecutionsPaymentTransaction, AutomaticExecutionsPaymentTransactionDto>(const AutomaticExecutionsPaymentTransactionDto& dto);

        template<>
        SuccessfulEndBatchExecutionTransaction fromDto<SuccessfulEndBatchExecutionTransaction, SuccessfulEndBatchExecutionTransactionDto>(const SuccessfulEndBatchExecutionTransactionDto& dto);

        template<>
        UnsuccessfulEndBatchExecutionTransaction fromDto<UnsuccessfulEndBatchExecutionTransaction, UnsuccessfulEndBatchExecutionTransactionDto>(const UnsuccessfulEndBatchExecutionTransactionDto& dto);

        /// Account Property Transactions
		template<>
		AccountTransactionPropertyTransaction fromDto<AccountTransactionPropertyTransaction, AccountTransactionPropertyTransactionDto >(const AccountTransactionPropertyTransactionDto & dto);

		template<>
		AccountMosaicPropertyTransaction fromDto<AccountMosaicPropertyTransaction, AccountMosaicPropertyTransactionDto >(const AccountMosaicPropertyTransactionDto & dto);

		template<>
		AccountAddressPropertyTransaction fromDto<AccountAddressPropertyTransaction, AccountAddressPropertyTransactionDto >(const AccountAddressPropertyTransactionDto & dto);

        /// Streaming

        template<>
        StreamStartTransaction fromDto<StreamStartTransaction, StreamStartTransactionDto >(const StreamStartTransactionDto & dto);

        template<>
        StreamFinishTransaction fromDto<StreamFinishTransaction, StreamFinishTransactionDto >(const StreamFinishTransactionDto & dto);

        template<>
        StreamPaymentTransaction fromDto<StreamPaymentTransaction, StreamPaymentTransactionDto >(const StreamPaymentTransactionDto & dto);

		/// Embedded Transactions

		template<>
		EmbeddedAccountLinkTransaction fromDto<EmbeddedAccountLinkTransaction, EmbeddedAccountLinkTransactionDto >(const EmbeddedAccountLinkTransactionDto & dto);

		template<>
		EmbeddedLockFundsTransaction fromDto<EmbeddedLockFundsTransaction, EmbeddedLockFundsTransactionDto >(const EmbeddedLockFundsTransactionDto & dto);

		template<>
		EmbeddedModifyMultisigAccountTransaction fromDto<EmbeddedModifyMultisigAccountTransaction, EmbeddedModifyMultisigAccountTransactionDto >(const EmbeddedModifyMultisigAccountTransactionDto & dto);

		template<>
		EmbeddedMosaicDefinitionTransaction fromDto<EmbeddedMosaicDefinitionTransaction, EmbeddedMosaicDefinitionTransactionDto >(const EmbeddedMosaicDefinitionTransactionDto & dto);

		template<>
		EmbeddedMosaicSupplyChangeTransaction fromDto<EmbeddedMosaicSupplyChangeTransaction, EmbeddedMosaicSupplyChangeTransactionDto >(const EmbeddedMosaicSupplyChangeTransactionDto & dto);

		template<>
		EmbeddedRegisterNamespaceTransaction fromDto<EmbeddedRegisterNamespaceTransaction, EmbeddedRegisterNamespaceTransactionDto >(const EmbeddedRegisterNamespaceTransactionDto & dto);

		template<>
		EmbeddedSecretLockTransaction fromDto<EmbeddedSecretLockTransaction, EmbeddedSecretLockTransactionDto >(const EmbeddedSecretLockTransactionDto & dto);

		template<>
		EmbeddedSecretProofTransaction fromDto<EmbeddedSecretProofTransaction, EmbeddedSecretProofTransactionDto >(const EmbeddedSecretProofTransactionDto & dto);

        template<>
        EmbeddedStoragePaymentTransaction fromDto<EmbeddedStoragePaymentTransaction, EmbeddedStoragePaymentTransactionDto >(const EmbeddedStoragePaymentTransactionDto & dto);

		template<>
		EmbeddedTransferTransaction fromDto<EmbeddedTransferTransaction, EmbeddedTransferTransactionDto >(const EmbeddedTransferTransactionDto & dto);

		template<>
		EmbeddedAliasTransactionBase fromDto<EmbeddedAliasTransactionBase, EmbeddedAliasTransactionBaseDto >(const EmbeddedAliasTransactionBaseDto & dto);

		template<>
		EmbeddedAddressAliasTransaction fromDto<EmbeddedAddressAliasTransaction, EmbeddedAddressAliasTransactionDto >(const EmbeddedAddressAliasTransactionDto & dto);

		template<>
		EmbeddedMosaicAliasTransaction fromDto<EmbeddedMosaicAliasTransaction, EmbeddedMosaicAliasTransactionDto >(const EmbeddedMosaicAliasTransactionDto & dto);

		template<>
		EmbeddedPrepareBcDriveTransaction fromDto<EmbeddedPrepareBcDriveTransaction, EmbeddedPrepareBcDriveTransactionDto >(const EmbeddedPrepareBcDriveTransactionDto & dto);

        template<>
        EmbeddedCreateLiquidityProviderTransaction fromDto<EmbeddedCreateLiquidityProviderTransaction, EmbeddedCreateLiquidityProviderTransactionDto >(const EmbeddedCreateLiquidityProviderTransactionDto & dto);

        template<>
        EmbeddedManualRateChangeTransaction fromDto<EmbeddedManualRateChangeTransaction, EmbeddedManualRateChangeTransactionDto >(const EmbeddedManualRateChangeTransactionDto & dto);

		template<>
		EmbeddedDataModificationTransaction fromDto<EmbeddedDataModificationTransaction, EmbeddedDataModificationTransactionDto >(const EmbeddedDataModificationTransactionDto & dto);

		template<>
		EmbeddedDownloadTransaction fromDto<EmbeddedDownloadTransaction, EmbeddedDownloadTransactionDto >(const EmbeddedDownloadTransactionDto & dto);

        template<>
        EmbeddedDownloadPaymentTransaction fromDto<EmbeddedDownloadPaymentTransaction, EmbeddedDownloadPaymentTransactionDto >(const EmbeddedDownloadPaymentTransactionDto & dto);

        template<>
        EmbeddedDriveClosureTransaction fromDto<EmbeddedDriveClosureTransaction, EmbeddedDriveClosureTransactionDto >(const EmbeddedDriveClosureTransactionDto & dto);

        template<>
		EmbeddedDataModificationApprovalTransaction fromDto<EmbeddedDataModificationApprovalTransaction, EmbeddedDataModificationApprovalTransactionDto >(const EmbeddedDataModificationApprovalTransactionDto & dto);

		template<>
		EmbeddedDataModificationCancelTransaction fromDto<EmbeddedDataModificationCancelTransaction, EmbeddedDataModificationCancelTransactionDto >(const EmbeddedDataModificationCancelTransactionDto & dto);

		template<>
		EmbeddedReplicatorOnboardingTransaction fromDto<EmbeddedReplicatorOnboardingTransaction, EmbeddedReplicatorOnboardingTransactionDto >(const EmbeddedReplicatorOnboardingTransactionDto & dto);

        template<>
        EmbeddedReplicatorOffboardingTransaction fromDto<EmbeddedReplicatorOffboardingTransaction, EmbeddedReplicatorOffboardingTransactionDto >(const EmbeddedReplicatorOffboardingTransactionDto & dto);

        template<>
        EmbeddedFinishDownloadTransaction fromDto<EmbeddedFinishDownloadTransaction, EmbeddedFinishDownloadTransactionDto >(const EmbeddedFinishDownloadTransactionDto & dto);

        /// Account Property Transactions
		template<>
		EmbeddedAccountTransactionPropertyTransaction fromDto<EmbeddedAccountTransactionPropertyTransaction, EmbeddedAccountTransactionPropertyTransactionDto >(const EmbeddedAccountTransactionPropertyTransactionDto & dto);

		template<>
		EmbeddedAccountMosaicPropertyTransaction fromDto<EmbeddedAccountMosaicPropertyTransaction, EmbeddedAccountMosaicPropertyTransactionDto >(const EmbeddedAccountMosaicPropertyTransactionDto & dto);

		template<>
		EmbeddedAccountAddressPropertyTransaction fromDto<EmbeddedAccountAddressPropertyTransaction, EmbeddedAccountAddressPropertyTransactionDto >(const EmbeddedAccountAddressPropertyTransactionDto & dto);

        /// Streaming

        template<>
        EmbeddedStreamStartTransaction fromDto<EmbeddedStreamStartTransaction, EmbeddedStreamStartTransactionDto >(const EmbeddedStreamStartTransactionDto & dto);

        template<>
        EmbeddedStreamFinishTransaction fromDto<EmbeddedStreamFinishTransaction, EmbeddedStreamFinishTransactionDto >(const EmbeddedStreamFinishTransactionDto & dto);

        template<>
        EmbeddedStreamPaymentTransaction fromDto<EmbeddedStreamPaymentTransaction, EmbeddedStreamPaymentTransactionDto >(const EmbeddedStreamPaymentTransactionDto & dto);

		/// Supercontract V2

		template<>
		EmbeddedDeployContractTransaction fromDto<EmbeddedDeployContractTransaction, EmbeddedDeployContractTransactionDto>(const EmbeddedDeployContractTransactionDto& dto);

		template<>
		EmbeddedManualCallTransaction fromDto<EmbeddedManualCallTransaction, EmbeddedManualCallTransactionDto>(const EmbeddedManualCallTransactionDto& dto);

		template<>
		EmbeddedAutomaticExecutionsPaymentTransaction fromDto<EmbeddedAutomaticExecutionsPaymentTransaction, EmbeddedAutomaticExecutionsPaymentTransactionDto>(const EmbeddedAutomaticExecutionsPaymentTransactionDto& dto);

		template<>
		EmbeddedSuccessfulEndBatchExecutionTransaction fromDto<EmbeddedSuccessfulEndBatchExecutionTransaction, EmbeddedSuccessfulEndBatchExecutionTransactionDto>(const EmbeddedSuccessfulEndBatchExecutionTransactionDto& dto);

		template<>
		EmbeddedUnsuccessfulEndBatchExecutionTransaction fromDto<EmbeddedUnsuccessfulEndBatchExecutionTransaction, EmbeddedUnsuccessfulEndBatchExecutionTransactionDto>(const EmbeddedUnsuccessfulEndBatchExecutionTransactionDto& dto);

        template<>
        ExtendedCallDigest fromDto<ExtendedCallDigest, ExtendedCallDigestDto>(const ExtendedCallDigestDto& dto);

        template<>
        Opinion fromDto<Opinion, OpinionDto>(const OpinionDto& dto);

        template<>
        SuperContractV2 fromDto<SuperContractV2, SuperContractV2Dto> (const SuperContractV2Dto& dto);
	}
}
}
}

