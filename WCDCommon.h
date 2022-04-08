#pragma once


#define WCD_DEVICE 0x8000

#define IOCTL_WCD_WRITE_ACCESS CTL_CODE(WCD_DEVICE, 0x800 , METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_WCD_READ_ACCESS CTL_CODE(WCD_DEVICE, 0x801 , METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_WCD_SET_THREAD_PRIORITY CTL_CODE(WCD_DEVICE, 0x802 , METHOD_NEITHER, FILE_ANY_ACCESS)

struct UserData {
    ULONG ThreadId;
    int   Priority;
};


