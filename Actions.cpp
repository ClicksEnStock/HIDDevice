
#include "Common.h"

void Extension::EnumerateAllDevices()
{
	// List the Devices
	hid_free_enumeration(deviceInfos);
	deviceInfos = hid_enumerate(0x0, 0x0);
	return;
}

void Extension::OpenDevice(int deviceId)
{
	if (curDevice && deviceId==curId)
	{
		return;
	}
	else
	{
		CloseDevice();
		struct hid_device_info *cur_dev;
		cur_dev = deviceInfos;
		int index = 1;
		//go to current device
		while (cur_dev && index!=deviceId) {
			index++;
			cur_dev = cur_dev->next;
		}
		curDevice = hid_open_path(cur_dev->path);

		if (!curDevice) 
		{
			//"Device Error", "Unable To Connect to Device");
			return;
		}
		hid_set_nonblocking(curDevice, 1);
		curId = deviceId;
	}
}

void Extension::CloseDevice()
{
	if (curDevice)
	{
		hid_close(curDevice);
		curDevice = NULL;
		curId = 0;
	}
}

//report is the whole string and should conatin Report ID inside
void Extension::SendReport(TCHAR* report, int formatType)
{
	BYTE outputbuf[MAX_STRING];
	int res;
	size_t inputN, outputN;
	struct hid_device_info *cur_dev;
	cur_dev = deviceInfos;
	int index = 1;

	if (!curId)
	{
		return;
	}
		
	if (formatType == 0)//hexadecimal
	{
		inputN = wcslen(report) / 2;
		//check that input is an hex string representation
		if ((wcslen(report) % 2) == 0)
		{
			//convert hex tchar* to byte
			for (int i = 0; i < inputN; i++)
			{
				_stscanf_s(&report[i * 2], _T("%2hhx"), &outputbuf[i]);
			}
			res = hid_write(curDevice, (unsigned char*)outputbuf, inputN);
		}
	}
	else if (formatType == 1)//ascii
	{
		inputN = wcslen(report)+1;
		for (int i = 0; i < inputN; i++)
		{
			outputbuf[i] = (unsigned char)report[i];
		}
		res = hid_write(curDevice, (unsigned char*)outputbuf, inputN);
	}
}
