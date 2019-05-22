
#include <nemcpp/crypto/private_key.h>
#include <nemcpp/crypto/secure_zero.h>

#include <algorithm>

namespace nem2_sdk {
	
	PrivateKey::PrivateKey(uint8_t* data, size_t size)
	{
		SecureZeroGuard guard({ data, size });
		
		if (size != key_.size()) {
			NEM2_SDK_THROW_1(crypto_error, "invalid private key size", size);
		}
		
		std::copy(data, data + size, key_.begin());
	}
	
	PrivateKey::PrivateKey(const supplier<uint8_t>& generator)
	{
		std::generate_n(key_.begin(), key_.end(), generator);
	}
	
	PrivateKey::PrivateKey(PrivateKey&& rhs): PrivateKey(rhs.key_.data(), rhs.key_.size())
	{ }
	
	PrivateKey::~PrivateKey()
	{
		SecureZero(key_);
	}
	
	PrivateKey& PrivateKey::operator=(PrivateKey&& rhs)
	{
		SecureZeroGuard guard(rhs.key_);
		key_ = rhs.key_;
		return *this;
	}
	
	const uint8_t* PrivateKey::data() const
	{
		return key_.data();
	}
	
	size_t PrivateKey::size() const
	{
		return key_.size();
	}
	
	bool operator==(const PrivateKey& lhs, const PrivateKey& rhs)
	{
		if (lhs.size() != rhs.size() || std::memcmp(lhs.data(), rhs.data(), lhs.size())) {
			return false;
		}
		
		return true;
	}
	
	bool operator!=(const PrivateKey& lhs, const PrivateKey& rhs)
	{
		return !(lhs == rhs);
	}
}
