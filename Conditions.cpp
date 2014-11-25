
#include "Common.h"

bool Extension::IsDeviceOpened(int deviceId)
{
    return (curId!=0);
}


bool Extension::OnDataReceived()
{
	if (curId)
	{
		readBytes = hid_read(curDevice, (unsigned char*)readBuffer, sizeof(readBuffer));
		return (readBytes > 0);
	}
	else
	{
		return false;
	}
}

