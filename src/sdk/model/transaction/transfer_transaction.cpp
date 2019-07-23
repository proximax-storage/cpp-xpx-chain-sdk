
#include <nemcpp/model/transaction/transfer_transaction.h>

namespace nem2_sdk {
	
	template<typename TBase>
	const Address& TTransferTransaction<TBase>::recipient() const
	{
		return recipient_;
	}
	
	template<typename TBase>
	const RawBuffer& TTransferTransaction<TBase>::message() const
	{
		// We do not return new buffer object in each call because we would like to allow usage like
		// 'for_each(tx.message().begin(), tx.message().end() ...)' and returning new object from this
		// method can lead to unexpected errors in the future because 'begin' and 'end' will be called
		// on different buffer objects (though it's currently will work).
		return messageBuffer_;
	}
	
	template<typename TBase>
	const std::string& TTransferTransaction<TBase>::messageStr() const
	{
		return message_;
	}
	
	template<typename TBase>
	const MosaicContainer& TTransferTransaction<TBase>::mosaics() const
	{
		return mosaics_;
	}
	
	template<typename TBase>
	Amount TTransferTransaction<TBase>::getAmount(MosaicId mosaicId) const
	{
		auto it = mosaics_.find(mosaicId);
		return it != mosaics_.end() ? it->amount : 0;
	}
	
	template class TTransferTransaction<Transaction>;
	template class TTransferTransaction<EmbeddedTransaction>;
}
