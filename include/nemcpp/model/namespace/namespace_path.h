
#pragma once

#include <nemcpp/model/namespace/namespace.h>

#include <string>
#include <string_view>
#include <vector>

namespace xpx_sdk {
	
	/// Namespace path.
	class NamespacePath {
	public:
		/// Creates namespace path from a namespace full name (like foo.bar.baz).
		/// \note Throws \c namespace_error if \a fullName does not represent valid namespace name.
		NamespacePath(std::string_view fullName);
		
		/// Returns namespace full name corresponding to path.
		const std::string& fullName() const;
		
		/// Returns number of nested namespaces.
		size_t size() const;
		
		/// Returns root namespace.
		const Namespace& root() const;
		
		/// Returns leaf namespace (outermost child namespace).
		const Namespace& leaf() const;
		
		/// Returns namespace at specified location \a pos 
		/// \note Throws \c namespace_error if \a pos is not within the range of namespace path.
		const Namespace& operator[](size_t pos) const;
		
		/// Returns a constant iterator to the root namespace.
		auto begin() const
		{ return subnamespaces_.begin(); }
		
		/// Returns a constant iterator to the element following the leaf namespace.
		auto end() const
		{ return subnamespaces_.end(); }
		
		/// Returns a constant iterator to the leaf namespace.
		auto rbegin() const
		{ return subnamespaces_.rbegin(); }
		
		/// Returns a constant iterator to the element following the root namespace.
		auto rend() const
		{ return subnamespaces_.rend(); }
		
	private:
		std::string fullName_;
		std::vector<Namespace> subnamespaces_;
	};
	
	/// Returns \c true if \a lhs and \a rhs are equal.
	bool operator==(const NamespacePath& lhs, const NamespacePath& rhs);
	
	/// Returns \c true if \a lhs and \a rhs are not equal.
	bool operator!=(const NamespacePath& lhs, const NamespacePath& rhs);
}
