
#include <nemcpp/model/namespace/namespace.h>
#include <nemcpp/crypto/hash.h>

namespace xpx_sdk {
	
	Namespace::Namespace(std::string_view name, NamespaceId parentId):
		id_(parentId == Namespace_Base_Id ? GenerateRootId(name) : GenerateChildId(name, parentId)),
		parentId_(parentId),
		name_(name)
	{ }
	
	NamespaceType Namespace::type() const
	{
		return parentId_ == Namespace_Base_Id ? NamespaceType::Root : NamespaceType::Child;
	}
	
	NamespaceId Namespace::id() const
	{
		return id_;
	}
	
	const std::string& Namespace::name() const
	{
		return name_;
	}
	
	NamespaceId Namespace::parentId() const
	{
		return parentId_;
	}
	
	NamespaceId Namespace::GenerateRootId(std::string_view name)
	{
		return GenerateChildId(name, Namespace_Base_Id);
	}
	
	NamespaceId Namespace::GenerateChildId(std::string_view name, NamespaceId parentId)
	{
		if (name.empty()) {
			NEM2_SDK_THROW_1(namespace_error, "invalid namespace name size", name.size());
		}
		
		constexpr uint64_t Namespace_Flag = 1ull << 63;
		auto hash = Sha3_256::Hash(parentId, name);
		return reinterpret_cast<const NamespaceId&>(*hash.data()) | Namespace_Flag;
	}
	
	bool operator==(const Namespace& lhs, const Namespace& rhs)
	{
		return lhs.id() == rhs.id();
	}
	
	bool operator!=(const Namespace& lhs, const Namespace& rhs)
	{
		return !(lhs == rhs);
	}
}
