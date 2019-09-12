
#pragma once

#include <nemcpp/types.h>

#include <string>
#include <string_view>

namespace xpx_sdk {
	
	/// Namespace type.
	enum class NamespaceType : uint8_t {
		Root,
		Child
	};
	
	/// Namespace.
	class Namespace {
	public:
		/// Creates namespace name.
		/// \note Throws \c namespace_error if \a name size is not valid.
		/// \note If \a parentId is not set, then root namespace is created.
		explicit Namespace(std::string_view name, NamespaceId parentId = Namespace_Base_Id);
		
		/// Returns namespace type.
		NamespaceType type() const;
		
		/// Returns namespace id.
		NamespaceId id() const;
		
		/// Returns namespace name.
		const std::string& name() const;
		
		/// Returns namespace parent id.
		NamespaceId parentId() const;
		
	public:
		/// Generates root namespace id from \a name.
		/// \note Throws \c namespace_error if \a name size is not valid.
		static NamespaceId GenerateRootId(std::string_view name);
		
		/// Generates child namespace id from \a name and \a parentId.
		/// \note Throws \c namespace_error if \a name size is not valid.
		static NamespaceId GenerateChildId(std::string_view name, NamespaceId parentId);
		
	private:
		NamespaceId id_;
		NamespaceId parentId_;
		std::string name_;
	};
	
	/// Returns \c true if \a lhs and \a rhs are equal.
	bool operator==(const Namespace& lhs, const Namespace& rhs);
	
	/// Returns \c true if \a lhs and \a rhs are not equal.
	bool operator!=(const Namespace& lhs, const Namespace& rhs);
}
