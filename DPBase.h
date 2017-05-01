#ifndef DPBase_h
#define DPBase_h

namespace DevilsParser 
{
	/* Define return codes */
	struct S_ReturnCodes 
	{
		enum RC 
		{
			eOK=0,
			eWarning,
			eError
		};
	};

	typedef S_ReturnCodes::RC ReturnStatus;
}

#endif
