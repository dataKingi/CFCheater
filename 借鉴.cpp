
#include <windows.h>
#include <atlstr.h>
//不掉血基质 cshell.dll + 2069F87
//窗口句柄基质 crossfire.exe+437204
//位置基质 cshell.dll+1995ECC

LONG 窗口句柄;
LONG 窗口基址;
LONG 不掉血基址;
LONG*写入数据;
LONG 位置基址;
LONG 游戏位置;
BOOL SoundSysDesc()//DLL函数
{
    return TRUE;
}
BOOL SoundSysMake()//DLL函数
{
    return TRUE;
}
VOID 不掉血()
{
    不掉血基址 = (LONG)GetModuleHandleA("cshell.dll") + 0x2069F87;
    窗口基址 = (LONG)GetModuleHandleA("crossfire.exe") + 0x437204;
    位置基址 = 不掉血基址 - 0x2069F87; 位置基址 = 位置基址 +0x1995ECC;
    while (true)//判断位置
    {
        游戏位置 = *(LONG*)位置基址;
        if (游戏位置==3)
        {
            break;
        }
        Sleep(300);
    }

    写入数据 = (LONG*)不掉血基址; *(LONG*)写入数据 = 1;
    //------------------------------------------
    窗口句柄 = *(LONG*)窗口基址;
    SetWindowTextA((HWND)窗口句柄, "[琴 单不掉血 版本5.5.3.0 黎歌 QQ2535096372]");

}
BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call,LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(NULL, NULL, LPTHREAD_START_ROUTINE(不掉血), NULL, NULL, NULL);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

