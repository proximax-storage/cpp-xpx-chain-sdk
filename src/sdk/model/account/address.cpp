
#include <nemcpp/model/account/address.h>
#include <nemcpp/crypto/hash.h>
#include <nemcpp/utils/base32.h>

#include <algorithm>
#include <cassert>

namespace nem2_sdk {
	
	namespace {
		constexpr size_t Checksum_Size = 4;
	}
	
	Address::Address(const AddressData& addressData):
		data_(addressData),
		encoded_(this),
		pretty_(this)
	{
		if (!IsValid(data_)) {
			NEM2_SDK_THROW(account_error, "invalid binary address");
		}
	}
	
	Address::Address(const Key& publicKey, NetworkIdentifier networkId):
		encoded_(this),
		pretty_(this)
	{
		auto publicKeyHash = Ripemd_160::Hash(Sha3_256::Hash(publicKey));
		
		data_[0] = to_underlying_type(networkId);
		std::copy(publicKeyHash.begin(), publicKeyHash.end(), data_.begin() + 1);
		
		auto checksumHash = Sha3_256::Hash(RawBuffer(data_.data(), Hash160_Size + 1));
		std::copy(checksumHash.begin(), checksumHash.begin() + Checksum_Size, data_.begin() + Hash160_Size + 1);
	}
	
	Address::Address(std::string_view encodedAddress):
		encoded_(this),
		pretty_(this)
	{
		if (!DecodeAndCheck(encodedAddress, data_)) {
			NEM2_SDK_THROW(account_error, "invalid encoded address");
		}
	}
	
	Address::Address(const Address& rhs):
		data_(rhs.data_),
		encoded_(this),
		pretty_(this)
	{
		encoded_ = rhs.encoded_;
		pretty_ = rhs.pretty_;
	}
	
	Address::Address(Address&& rhs):
		data_(std::move(rhs.data_)),
		encoded_(this),
		pretty_(this)
	{
		encoded_ = std::move(rhs.encoded_);
		pretty_ = std::move(rhs.pretty_);
	}
	
	NetworkIdentifier Address::networkId() const
	{
		return static_cast<NetworkIdentifier>(data_[0]);
	}
	
	const AddressData& Address::binary() const
	{
		return data_;
	}
	
	const std::string& Address::encoded() const
	{
		return *encoded_;
	}
	
	const std::string& Address::pretty() const
	{
		return *pretty_;
	}
	
	const uint8_t* Address::data() const
	{
		return data_.data();
	}
	
	size_t Address::size() const
	{
		return data_.size();
	}
	
	bool Address::IsValid(const AddressData& addressData)
	{
		NetworkIdentifier networkId = static_cast<NetworkIdentifier>(addressData[0]);
		
		switch (networkId) {
		case NetworkIdentifier::Mijin:
		case NetworkIdentifier::Mijin_Test:
		case NetworkIdentifier::Public:
		case NetworkIdentifier::Public_Test:
			break;
		default:
			return false;
		}
		
		auto checksumBegin = Address_Size - Checksum_Size;
		auto checksumHash = Sha3_256::Hash(RawBuffer(addressData.data(), checksumBegin));
		
		return std::equal(checksumHash.begin(), checksumHash.begin() + Checksum_Size,
		                  addressData.begin() + checksumBegin);
	}
	
	bool Address::IsValidEncoded(std::string_view encodedAddress)
	{
		AddressData addressData;
		return DecodeAndCheck(encodedAddress, addressData);
	}
	
	bool Address::DecodeAndCheck(std::string_view encodedAddress, AddressData& addressData)
	{
		std::string encodedData(encodedAddress.data(), encodedAddress.size());
		
		// Removing all "-" symbols used in pretty address format.
		encodedData.erase(std::remove(encodedData.begin(), encodedData.end(), '-'), encodedData.end());
		
		if (!Base32::Decode(encodedData, addressData)) {
			return false;
		}
		
		return IsValid(addressData);
	}
	
	void Address::initEncodedAddress(std::optional<std::string>& value)
	{
		value = Base32::Encode(data_);
	}
	
	void Address::initPrettyAddress(std::optional<std::string>& value)
	{
		assert(encoded_->size() == Address_Encoded_Size);
		
		auto encodedData = encoded_->data();
		std::string pretty;
		
		pretty.append(encodedData, 6);
		pretty.append(1, '-');
		pretty.append(encodedData + 6, 6);
		pretty.append(1, '-');
		pretty.append(encodedData + 2*6, 6);
		pretty.append(1, '-');
		pretty.append(encodedData + 3*6, 6);
		pretty.append(1, '-');
		pretty.append(encodedData + 4*6, 6);
		pretty.append(1, '-');
		pretty.append(encodedData + 5*6, 6);
		pretty.append(1, '-');
		pretty.append(encodedData + 6*6, 4); //checksum
		
		value = std::move(pretty);
	}
	
	bool operator==(const Address& lhs, const Address& rhs)
	{
		return lhs.binary() == rhs.binary();
	}
	
	bool operator!=(const Address& lhs, const Address& rhs)
	{
		return !(lhs == rhs);
	}
}
