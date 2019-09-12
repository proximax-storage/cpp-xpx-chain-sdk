
#include <xpxchaincpp/model/namespace/namespace_path.h>

namespace xpx_chain_sdk {
	
	NamespacePath::NamespacePath(std::string_view fullName): fullName_{ fullName }
	{
		size_t pos = 0;
		size_t prevPos = 0;
		NamespaceId prevParentId = Namespace_Base_Id;
		
		do {
			pos = fullName.find('.', prevPos);
			auto name = fullName.substr(0, pos);
			subnamespaces_.emplace_back(name, prevParentId);
			
			prevPos = pos + 1;
			prevParentId = subnamespaces_.back().id();
		} while (pos != std::string::npos);
	}
	
	const std::string& NamespacePath::fullName() const
	{
		return fullName_;
	}
	
	size_t NamespacePath::size() const
	{
		return subnamespaces_.size();
	}
	
	const Namespace& NamespacePath::root() const
	{
		return (*this)[0];
	}
	
	const Namespace& NamespacePath::leaf() const
	{
		return (*this)[size() - 1];
	}
	
	const Namespace& NamespacePath::operator[](size_t pos) const
	{
		if (pos >= size()) {
			XPX_CHAIN_SDK_THROW_2(namespace_error, "namespace path index is out of range", pos, size());
		}
		
		return subnamespaces_[pos];
	}
	
	bool operator==(const NamespacePath& lhs, const NamespacePath& rhs)
	{
		return lhs.fullName() == rhs.fullName();
	}
	
	bool operator!=(const NamespacePath& lhs, const NamespacePath& rhs)
	{
		return !(lhs == rhs);
	}
}
