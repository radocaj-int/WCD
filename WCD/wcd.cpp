
#include <ntifs.h>
#include <ntddk.h>

extern "C"
{
#include"WCDCommon.h"

    ULONG global_object;

    void
        DriverUnload(_In_ PDRIVER_OBJECT pdriver)
    {
        KdPrint(("WCD unload."));
        UNICODE_STRING symLink = RTL_CONSTANT_STRING(L"\\??\\WCD");
        IoDeleteSymbolicLink(&symLink);

        IoDeleteDevice(pdriver->DeviceObject);
        return;
    }

    NTSTATUS
        WCDCreateClose(_In_ PDEVICE_OBJECT pdevice, _In_ PIRP Irp)
    {
        UNREFERENCED_PARAMETER(pdevice);

        Irp->IoStatus.Status = STATUS_SUCCESS;
        Irp->IoStatus.Information = 0;
        IoCompleteRequest(Irp, IO_NO_INCREMENT);
        return STATUS_SUCCESS;
    }

    NTSTATUS
        WCDDeviceControl(_In_ PDEVICE_OBJECT pdevice, _In_ PIRP Irp)
    {
        UNREFERENCED_PARAMETER(pdevice);
        auto stack = IoGetCurrentIrpStackLocation(Irp);
        auto status = STATUS_SUCCESS;

        switch (stack->Parameters.DeviceIoControl.IoControlCode) {

        case IOCTL_WCD_WRITE_ACCESS: {
            ULONG tmp_write = global_object++;
            KdPrint(("after writing global_object = %ul", tmp_write));
            break;
        }
        case IOCTL_WCD_READ_ACCESS: {
            ULONG tmp_read = global_object;
            if (tmp_read)
                KdPrint(("After reading, global_object =%ul", tmp_read));

            break;
        }
        case IOCTL_WCD_SET_THREAD_PRIORITY: {
            
            auto length = stack->Parameters.DeviceIoControl.InputBufferLength;
            if (length < sizeof(UserData)) {
                status = STATUS_BUFFER_TOO_SMALL;
                break;
            }

            auto data = (UserData*)stack->Parameters.DeviceIoControl.Type3InputBuffer;
            if (data == nullptr) {
                status = STATUS_INVALID_PARAMETER;
                break;
            }

            if (data->Priority < 1 || data->Priority >31) {
                status = STATUS_INVALID_PARAMETER;
                break;
            }

            PETHREAD thread;
            status = PsLookupThreadByThreadId(ULongToHandle(data->ThreadId), &thread);
            if (!NT_SUCCESS(status)) {
                break;
            }

            KeSetPriorityThread((PKTHREAD)thread, data->Priority);
            ObDereferenceObject(thread);
            KdPrint(("threadid %d has changed priority to %d ", data->ThreadId, data->ThreadId));

            break;
            }

        default:
            break;
        }

        Irp->IoStatus.Status = status;
        Irp->IoStatus.Information = 0;
        IoCompleteRequest(Irp, IO_NO_INCREMENT);

        return status;
    }

    NTSTATUS
        DriverEntry(_In_ PDRIVER_OBJECT pdriver, _In_ PUNICODE_STRING RegistryPath)
    {
        UNREFERENCED_PARAMETER(RegistryPath);
        KdPrint(("WCD Driver Entry!"));

        UNICODE_STRING devName = RTL_CONSTANT_STRING(L"\\Device\\WCD");
        PDEVICE_OBJECT DeviceObject;
        NTSTATUS status = IoCreateDevice(pdriver,
            0,
            &devName,
            FILE_DEVICE_UNKNOWN,
            0,
            FALSE,
            &DeviceObject);

        if (!NT_SUCCESS(status)) {
            KdPrint(("Failed to create device object for WCD"));
            // TODO: DeviceObject could be NULL
            IoDeleteDevice(DeviceObject);
            return status;
        }

        UNICODE_STRING symLink = RTL_CONSTANT_STRING(L"\\??\\WCD");
        status = IoCreateSymbolicLink(&symLink, &devName);
        if (!NT_SUCCESS(status)) {
            KdPrint(("Failed to create symbolic link"));
            IoDeleteDevice(DeviceObject);
            return status;
        }
        pdriver->MajorFunction[IRP_MJ_CREATE] = WCDCreateClose;
        pdriver->MajorFunction[IRP_MJ_CLOSE] = WCDCreateClose;
        pdriver->MajorFunction[IRP_MJ_DEVICE_CONTROL] = WCDDeviceControl;
        pdriver->DriverUnload = DriverUnload;
        return STATUS_SUCCESS;
    }

}