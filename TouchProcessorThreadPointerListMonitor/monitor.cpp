#include <windows.h>

#include <cstdio>

#include "monitor.h"

/* static */
Monitor& Monitor::Get() {
	static Monitor instance;
	return instance;
}

Monitor::Monitor()
{
}

void Monitor::OnUpdateStart(const EVENT_RECORD& aEventRecord)
{
	wprintf(L"[%lu] > UPDATE\n", aEventRecord.EventHeader.ThreadId);
}

void Monitor::OnUpdateEnd(const EVENT_RECORD& aEventRecord, bool aNoIter, bool aEmptyList)
{
	wprintf(L"[%lu] < UPDATE\n", aEventRecord.EventHeader.ThreadId);
}

void Monitor::OnAllocAndLinkStart(const EVENT_RECORD& aEventRecord)
{
	wprintf(L"[%lu] > ALLOC\n", aEventRecord.EventHeader.ThreadId);
}

void Monitor::OnAllocAndLinkEnd(const EVENT_RECORD& aEventRecord)
{
	wprintf(L"[%lu] < ALLOC\n", aEventRecord.EventHeader.ThreadId);
}

void Monitor::OnUnlinkAndFreeStart(const EVENT_RECORD& aEventRecord)
{
	wprintf(L"[%lu] > FREE\n", aEventRecord.EventHeader.ThreadId);
}

void Monitor::OnUnlinkAndFreeEnd(const EVENT_RECORD& aEventRecord)
{
	wprintf(L"[%lu] < FREE\n", aEventRecord.EventHeader.ThreadId);
}
