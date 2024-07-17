#include <krabs.hpp>

#include <windows.h>
#include <evntcons.h>

#include "monitor.h"

#define TRACE_SESSION_NAME L"TouchProcessorThreadPointerListMonitor"
#define WIN32KBASE_WPP_CONTROL_GUID L"{0f81ec00-9e52-48e6-b899-eb3bbeede741}"

const GUID kUpdateListProviderGuid{
	0x183337b3,
	0xa321,
	0x3e44,
	{0x79, 0xf0,
	0x22, 0x72, 0x39, 0x9c, 0x58, 0x3b}
};

enum UpdateListEventId {
	Update_Start = 0x71,
	Update_End_NoIter = 0x72,
	Update_End_EmptyList = 0x73,
	Update_End_Iter = 0x74,
};

const GUID kLinkUnlinkProviderGuid{
	0x27299bb4,
	0xfcd8,
	0x3cf9,
	{0xf0, 0x8d,
	0xa9, 0x0e, 0x79, 0xa2, 0xf7, 0x0f}
};

enum LinkUnlinkEventId {
	Find_Start = 0xf6,
	Find_End = 0xf7,
	AllocAndLink_Start = 0xf8,
	AllocAndLink_End = 0xf9,
	Alloc_Start = 0xfa,
	Alloc_End = 0xfb,
	UnlinkAndFree_Start = 0xfc,
	UnlinkAndFree_End = 0xfd,
};

void OnEvent(const EVENT_RECORD& aRecord, const krabs::trace_context& aTraceContext)
{
	Monitor& monitor = Monitor::Get();
	if (aRecord.EventHeader.ProviderId == kUpdateListProviderGuid) {
		switch (aRecord.EventHeader.EventDescriptor.Id) {
		case Update_Start:
			monitor.OnUpdateStart(aRecord);
			break;
		case Update_End_NoIter:
		case Update_End_EmptyList:
		case Update_End_Iter:
			monitor.OnUpdateEnd(
				aRecord,
				aRecord.EventHeader.EventDescriptor.Id == Update_End_NoIter,
				aRecord.EventHeader.EventDescriptor.Id == Update_End_EmptyList
			);
			break;
		default:
			;
		}
	}
	else if (aRecord.EventHeader.ProviderId == kLinkUnlinkProviderGuid) {
		switch (aRecord.EventHeader.EventDescriptor.Id) {
		case AllocAndLink_Start:
			monitor.OnAllocAndLinkStart(aRecord);
			break;
		case AllocAndLink_End:
			monitor.OnAllocAndLinkEnd(aRecord);
		case UnlinkAndFree_Start:
			monitor.OnUnlinkAndFreeStart(aRecord);
			break;
		case UnlinkAndFree_End:
			monitor.OnUnlinkAndFreeEnd(aRecord);
		default:
			;
		}
	}
}

int main()
{
	krabs::provider<> control(krabs::guid(WIN32KBASE_WPP_CONTROL_GUID));
	control.any(0xFFFFFFFFFFFFFFFFUi64);
	control.level(0xFF);

	krabs::provider<> dummy(krabs::emptyGuid);
	dummy.add_on_event_callback(OnEvent);

	krabs::user_trace trace(TRACE_SESSION_NAME);
	trace.enable(control);
	trace.enable(dummy);

	trace.start();

	return 0;
}
