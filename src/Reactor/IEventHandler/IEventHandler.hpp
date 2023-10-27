

#include "AEventHandler.h"

namespace reactor {

	class AEventHandler {
	   private:
	   protected:
	   public:
		AEventHandler(){};
		virtual ~AEventHandler(){};
		virtual handle_t getHandle() const = 0;
		virtual void handleRead() = 0;
		virtual void handleWrite() = 0;
		virtual void handleError() = 0;
	};

}  // namespace reactor
