
class Extension
{
public:

    LPRDATA rdPtr;
    LPRH    rhPtr;

    Edif::Runtime Runtime;

    static const int MinimumBuild = 251;
    static const int Version = 1;

    static const int OEFLAGS = 0;
    static const int OEPREFS = 0;
    
    static const int WindowProcPriority = 100;

    Extension(LPRDATA rdPtr, LPEDATA edPtr, fpcob cobPtr);
    ~Extension();


    /*  Add any data you want to store in your extension to this class
        (eg. what you'd normally store in rdPtr)

        Unlike rdPtr, you can store real C++ objects with constructors
        and destructors, without having to call them manually or store
        a pointer.
    */
	hid_device		*curDevice;
	hid_device_info *deviceInfos;
	int curId;
	int readBytes;
	unsigned char readBuffer[MAX_STRING];
	TCHAR lastError[MAX_ERROR_STRING];
	//vector<unsigned char> byteBuf;
	// int MyVariable;

	TCHAR* GetInfoString(int deviceId, int type);
	unsigned short GetInfoId(int deviceId, int type);

    /*  Add your actions, conditions and expressions as real class member
        functions here. The arguments (and return type for expressions) must
        match EXACTLY what you defined in the JSON.

        Remember to link the actions, conditions and expressions to their
        numeric IDs in the class constructor (Extension.cpp)
    */

    /// Actions
	void EnumerateAllDevices();
	void OpenDevice(int deviceId);
	void CloseDevice();
	void SendReport(TCHAR* report, int formatType);
	void SendReportWithId(TCHAR* report, int formatType, int reportId);
	
    /// Conditions
	bool IsDeviceOpened(int deviceId);
	bool OnDataReceived();

    /// Expressions
	int DeviceCount();
	TCHAR* InputBuffer(int formatType);
	TCHAR* ManufacturerName(int deviceId);
	TCHAR* ProductName(int deviceId);
	TCHAR* SerialNumber(int deviceId);
	TCHAR* ProductId(int deviceId);
	TCHAR* VendorId(int deviceId);
	TCHAR* LastError();
	int OutputBufferLength();

    /* These are called if there's no function linked to an ID */

    void Action(int ID, LPRDATA rdPtr, long param1, long param2);
    long Condition(int ID, LPRDATA rdPtr, long param1, long param2);
    long Expression(int ID, LPRDATA rdPtr, long param);




    /*  These replace the functions like HandleRunObject that used to be
        implemented in Runtime.cpp. They work exactly the same, but they're
        inside the extension class.
    */

    short Handle();
    short Display();

    short Pause();
    short Continue();

    bool Save(HANDLE File);
    bool Load(HANDLE File);

};