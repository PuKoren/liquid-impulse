#if PSP
#include <pspkernel.h>
#include <pspdebug.h>
#endif
#include <iostream>
#include "Application.h"


#if PSP
// De rappel de service de sortie
int exit_callback(int arg1, int arg2, void *common)
{
	running = false;
	sceKernelExitGame();
	return 0;
}


// Rappel de service
int CallbackThread(SceSize args, void *argp)
{
 int cbid;
 cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
 sceKernelRegisterExitCallback(cbid);
 
 sceKernelSleepThreadCB();
 
 return 0;
}


// Monte le Rappel de service et envoi son ID
int SetupCallbacks(void)
{
 int thid = 0;
 thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, 0);
 
 if (  thid >= 0 )
 {
  sceKernelStartThread(thid, 0, 0);
 }
 
 return thid;
}

extern "C"
#endif
int main(int argc, char** argv){
	Application a;
	a.Run();
	#if PSP
    sceKernelExitGame();
    #endif
	return 0;
}
