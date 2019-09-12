
#pragma once

namespace xpx_chain_sdk {
	
	/// Mixin that disables copy/move for descendants.
	class NonCopyable {
	protected:
		/// Default constructor.
		NonCopyable() = default;
		
		/// Default destructor.
		~NonCopyable() = default;
		
		/// Disabled copy constructor.
		NonCopyable(const NonCopyable&) = delete;
		
		/// Disabled move constructor
		NonCopyable(NonCopyable&&) = delete;
		
		/// Disabled assignment operator.
		NonCopyable& operator=(const NonCopyable&) = delete;
		
		/// Disabled move-assignment operator.
		NonCopyable& operator=(NonCopyable&&) = delete;
	};
	
	/// Mixin that allows only moving for descendants.
	class MoveOnly {
	protected:
		/// Default constructor.
		MoveOnly() = default;
		
		/// Default destructor.
		~MoveOnly() = default;
		
		/// Disabled copy constructor.
		MoveOnly(const MoveOnly&) = delete;
		
		/// Default move constructor
		MoveOnly(MoveOnly&&) = default;
		
		/// Disabled assignment operator.
		MoveOnly& operator=(const MoveOnly&) = delete;
		
		/// Default move-assignment operator.
		MoveOnly& operator=(MoveOnly&&) = default;
	};
}
