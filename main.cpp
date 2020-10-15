#pragma comment (lib, "setupapi.lib")
#include <iostream>
#include <windows.h>
#include <setupapi.h>
#include <string>
#include <clocale>
#include <iomanip>

using namespace std;

string GetDevicePropertyRegistry(HDEVINFO device_info_set, SP_DEVINFO_DATA device_info_data, DWORD _property)
{
    DWORD reg_data_type;
    PBYTE buffer = nullptr;
    DWORD buffer_size = 0;

    while (!SetupDiGetDeviceRegistryProperty(
        device_info_set,
        &device_info_data,
        _property,											
        &reg_data_type,
        buffer,
        buffer_size,
        &buffer_size))
    {
        if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
        {
            if (buffer)
            {
                delete buffer;
                buffer = nullptr;
                buffer_size *= 2;
            }
            buffer = new BYTE[buffer_size + 1];
        }
        else break;
    }

    string result((char*)buffer);

    delete[] buffer;

    return result;
}

int main()
{
    setlocale(LC_ALL, "Russian");

    HDEVINFO device_info_set;				
    SP_DEVINFO_DATA device_info_data;			
    DWORD i;

    device_info_set = SetupDiGetClassDevs(
        NULL,
        "PCI",
        0,
        DIGCF_PRESENT |							
        DIGCF_ALLCLASSES						//возвращает устройства, установленные в системе
    );

    if (device_info_set == INVALID_HANDLE_VALUE)
    {
        exit(EXIT_FAILURE);
    }

    device_info_data.cbSize = sizeof(SP_DEVINFO_DATA);

    cout << left << setw(70) << "Device name" << setw(40) << "Manufacturer" << setw(10) << "DeviceID" << setw(10) << "VendorID" << endl;
    for (i = 0; SetupDiEnumDeviceInfo(device_info_set, i, &device_info_data); i++)
    {
        string vendorID, deviceID, result, manufacturer;

        result = GetDevicePropertyRegistry(device_info_set, device_info_data, SPDRP_COMPATIBLEIDS); //возвращает строку с id устройств
        vendorID = result.substr(8, 4);
        deviceID = result.substr(17, 4);
        result = GetDevicePropertyRegistry(device_info_set, device_info_data, SPDRP_DEVICEDESC);    //возвращает имя устройства
        manufacturer = GetDevicePropertyRegistry(device_info_set, device_info_data, SPDRP_MFG);     //возвращает производителя устройства
        cout << left << setw(70) << result << setw(40) << manufacturer << setw(10) << deviceID  << setw(10) << vendorID << endl;
    }

    if (GetLastError() != NO_ERROR && GetLastError() != ERROR_NO_MORE_ITEMS)
    {
        exit(EXIT_FAILURE);
    }

    SetupDiDestroyDeviceInfoList(device_info_set);      //удаляет набор информации об устройстве и освобождает всю связанную с ним память

    //system("pause");

    return 0;
}