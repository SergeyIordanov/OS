#pragma once

//Constants
#define IHV INVALID_HANDLE_VALUE

#define GR GENERIC_READ

#define GW GENERIC_WRITE

#define FSR FILE_SHARE_READ

#define OE OPEN_EXISTING

#define CN CREATE_NEW

#define OA OPEN_ALWAYS

//Macroses
#define CreateFileForRead(fname) CreateFile(fname, GR, FSR, 0, OE, 0, 0)

#define CreateFileForWrite(fname) CreateFile(fname, GW, FSR, 0, CN, 0, 0)

#define CreateFileForReadWrite(fname) CreateFile(fname, GR|GW, FSR, 0, OA, 0, 0)

#define OpenFileForWrite(fname) CreateFile(fname, GW, FSR, 0, OE, 0, 0)
