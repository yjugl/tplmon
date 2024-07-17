#ifndef __MONITOR_H_
#define __MONITOR_H_

#include <windows.h>
#include <evntcons.h>

class Monitor {
public:
	static Monitor& Get();

public:
	void OnUpdateStart(const EVENT_RECORD& aEventRecord);
	void OnUpdateEnd(const EVENT_RECORD& aEventRecord, bool aNoIter, bool aEmptyList);
	void OnAllocAndLinkStart(const EVENT_RECORD& aEventRecord);
	void OnAllocAndLinkEnd(const EVENT_RECORD& aEventRecord);
	void OnUnlinkAndFreeStart(const EVENT_RECORD& aEventRecord);
	void OnUnlinkAndFreeEnd(const EVENT_RECORD& aEventRecord);

private:
	Monitor();
};

#endif  // __MONITOR_H_
