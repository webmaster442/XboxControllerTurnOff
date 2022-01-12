#include <windows.h>
#include <xinput.h>
#include <string.h>
#include <stdbool.h>

#pragma comment(lib, "XInput.lib")

#define QUESTION "Do you want to turn off Xbox360 controllers?"
#define HELP "--h, --help : Display this message\r\n1 : Turn off Controller 1\r\n2 : Turn off Controller 2\r\n3 : Turn off Controller 3\r\n4 : Turn off Controller 4"
#define TITLE "XboxControllerTurnOff"

typedef DWORD (WINAPI* XInputPowerOffController_t)(DWORD i);

void TurnOffAll(HINSTANCE hXInputDLL)
{
	int result = MessageBox(NULL, QUESTION, TITLE, MB_YESNO | MB_ICONQUESTION);
	if (result == IDYES) 
	{
		for(short i=0; i<4; ++i)
		{
			XINPUT_STATE state;
        	memset(&state, 0, sizeof(XINPUT_STATE));
			XInputPowerOffController_t turnOff = (XInputPowerOffController_t)GetProcAddress(hXInputDLL, (LPCSTR) 103);
        	if(XInputGetState(i, &state) == ERROR_SUCCESS)
        	{
            	turnOff(i);
        	}
        	ZeroMemory(&state, sizeof(XINPUT_STATE));
		}
	}
}

void TurnOffSpecific(HINSTANCE hXInputDLL, int controllerIndex)
{
	XINPUT_STATE state;
    memset(&state, 0, sizeof(XINPUT_STATE));
	XInputPowerOffController_t turnOff = (XInputPowerOffController_t)GetProcAddress(hXInputDLL, (LPCSTR) 103);
    if(XInputGetState(controllerIndex, &state) == ERROR_SUCCESS)
    {
        turnOff(controllerIndex);
    }
    ZeroMemory(&state, sizeof(XINPUT_STATE));
}

bool HelpRequested(int argc, char** argv)
{
	for (int i=0; i<argc; i++)
	{
		if (strncmp(argv[i], "-h", 3) == 0
			|| strncmp(argv[i], "--help", 7) == 0)
		{
			return true;
		}
	}
	return false;
}


int main(int argc, char** argv)
{
	HINSTANCE hXInputDLL = LoadLibraryA("XInput1_4.dll");
	if (hXInputDLL == NULL)
	{
		MessageBox(NULL, "Xinput load failed", TITLE, MB_OK | MB_ICONERROR);
		return 1;
	}

	int exitCode = 0; 

	if (argc < 2)
	{
		TurnOffAll(hXInputDLL);
	}
	else if (argc > 2 
			|| HelpRequested(argc, argv))
	{
		MessageBox(NULL, HELP, TITLE, MB_OK | MB_ICONINFORMATION);
	}
	else
	{
		long index = strtoll(argv[1], NULL, 10);
		if (index < 1 || index > 4)
		{
			MessageBox(NULL, "Invalid controller: Controller index can be 1, 2, 3 or 4", TITLE, MB_OK | MB_ICONERROR);
			exitCode = 1;
		}
		else
		{
			TurnOffSpecific(hXInputDLL, (int)(index - 1));
		}
	}
	FreeLibrary(hXInputDLL);
	return exitCode;
}