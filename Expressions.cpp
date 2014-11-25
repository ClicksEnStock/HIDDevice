
#include "Common.h"

int Extension::DeviceCount()
{
	struct hid_device_info *cur_dev = nullptr;
	cur_dev = deviceInfos;
	int index = 1;
	while (cur_dev) {
		index++;
		cur_dev = cur_dev->next;
	}
	return (index-1);
}

TCHAR*  Extension::InputBuffer(int formatType)
{
	if (curId)
	{
		TCHAR out[2048] = { 0 };
		if (readBytes <= 0)
		{
			readBytes = hid_read(curDevice, (unsigned char*)readBuffer, sizeof(readBuffer));
		}
		if (formatType == 0)//hexadecimal
		{
			for (int i = 0; i < readBytes; i++)
			{
				_stprintf_s(&out[i*2], 3 , _T("%02X"), readBuffer[i]);
			}
		}
		else if(formatType == 1)//ascii
		{
			for (int i = 0; i < readBytes; i++)
			{
				_stprintf_s(&out[i], 2, _T("%c"), readBuffer[i]);
			}
		}
		else
		{
			return _T("");
		}
		readBytes = 0;
		return out;
	}
	else
	{
		return _T("");
	}
}

TCHAR*  Extension::ManufacturerName(int deviceId)
{
	try
	{
		return GetInfoString(deviceId, 0);
	}
	catch (exception& e)
	{
		return _T("Exception thrown");
	}
}

TCHAR*  Extension::ProductName(int deviceId)
{
	try
	{
		return GetInfoString(deviceId, 1);
	}
	catch (exception& e)
	{
		return _T("Exception thrown");
	}
}

TCHAR*  Extension::SerialNumber(int deviceId)
{
	try
	{
		return GetInfoString(deviceId, 2);
	}
	catch (exception& e)
	{
		return _T("Exception thrown");
	}
}

TCHAR* Extension::ProductId(int deviceId)
{
	try
	{
		unsigned short pId = GetInfoId(deviceId, 0);
		unsigned char b[2] = { 0 };
		TCHAR out[MAX_STRING] = { 0 };
		b[1] = pId & 0xff;
		b[0] = (pId >> 8) & 0xff;

		_stprintf_s(out, _countof(out), _T("%02X%02X"), b[0], b[1]);

		return out;
	}
	catch (exception& e)
	{
		return _T("Exception thrown");
	}
}

TCHAR* Extension::VendorId(int deviceId)
{
	try{
		unsigned short vId = GetInfoId(deviceId, 1);
		unsigned char b[2] = { 0 };
		TCHAR out[MAX_STRING] = { 0 };
		b[1] = vId & 0xff;
		b[0] = (vId >> 8) & 0xff;
	
		_stprintf_s(out, _countof(out), _T("%02X%02X"), b[0],b[1]);

		return out;
	}
	catch (exception& e)
	{
		return _T("Exception thrown");
	}
}

TCHAR* Extension::GetInfoString(int deviceId, int type)
{
	
	struct hid_device_info *cur_dev;
	cur_dev = deviceInfos;
	int index = 1;
	const int MAX_NAME_LENGTH = 126;
	while (cur_dev) {
		if (index == deviceId)
		{
			switch (type)
			{
			case 0:
				if (cur_dev->manufacturer_string && wcslen(cur_dev->manufacturer_string) <= MAX_NAME_LENGTH)
				{
					return cur_dev->manufacturer_string;
				}
				else
				{
					return _T("");
				}
				break;
			case 1:
				if (cur_dev->product_string && wcslen(cur_dev->product_string) <= MAX_NAME_LENGTH)
				{
					return cur_dev->product_string;
				}
				else
				{
					return _T("");
				}
				break;
			case 2:
				if (cur_dev->serial_number && wcslen(cur_dev->serial_number) <= MAX_NAME_LENGTH)
				{
					return cur_dev->serial_number;
				}
				else
				{
					return _T("");
				}
				break;
			default:
				return _T("");
				break;
			}
		}
		index++;
		cur_dev = cur_dev->next;
	}
	return _T("");
}

unsigned short Extension::GetInfoId(int deviceId, int type)
{
	struct hid_device_info *cur_dev = nullptr;
	cur_dev = deviceInfos;
	int index = 1;
	while (cur_dev) {
		if (index == deviceId)
		{
			return (type == 0) ? cur_dev->product_id : (cur_dev->vendor_id);
		}
		index++;
		cur_dev = cur_dev->next;
	}
	return 0;
}

TCHAR* Extension::LastError()
{
	try
	{
		if (curId && hid_error(curDevice)!=NULL)
		{
			_snwprintf_s(lastError, _countof(lastError), MAX_ERROR_STRING, hid_error(curDevice));
			return lastError;
		}
		else{
			return _T("");
		}
	}
	catch (exception& e)
	{
		return _T("Exception thrown");
	}
}

int Extension::OutputBufferLength()
{
	if (curId)
	{
		return hid_get_outputBufferLength(curDevice);
	}
}