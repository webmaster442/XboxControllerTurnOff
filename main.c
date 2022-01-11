#include <windows.h>
#include <xinput.h>

#pragma comment(lib, "XInput.lib")

#define QUESTION "Do you want to turn off Xbox360 controllers?"
#define TITLE "XboxControllerTurnOff"

void SearchAndTurnOffControllers(HINSTANCE hXInputDLL)
{
	for(short i=0; i<4; ++i)
	{
		XINPUT_STATE state;
        memset(&state, 0, sizeof(XINPUT_STATE));
        if(XInputGetState(i,&state)==ERROR_SUCCESS)
        {
            typedef DWORD (WINAPI* XInputPowerOffController_t)(DWORD i);
            XInputPowerOffController_t realXInputPowerOffController=(XInputPowerOffController_t) GetProcAddress(hXInputDLL, (LPCSTR) 103);
            realXInputPowerOffController(i);
        }
        ZeroMemory(&state, sizeof(XINPUT_STATE));
	}
}

int main()
{
	int result = MessageBox(NULL, QUESTION, TITLE, MB_YESNO | MB_ICONQUESTION);
	if (result == IDYES) 
	{
		HINSTANCE hXInputDLL = LoadLibraryA("XInput1_4.dll");
		if (hXInputDLL == NULL)
		{
			MessageBox(NULL, "Xinput load failed", TITLE, MB_OK | MB_ICONERROR);
			return 1;
		}
		SearchAndTurnOffControllers(hXInputDLL);
		FreeLibrary(hXInputDLL);
	}
	return 0;
}