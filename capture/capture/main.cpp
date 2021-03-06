// capture.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "capture.h"
#include <stdio.h>

#define CHECK_RETURN(ret) \
if (ret != 0) \
{ \
    printf("ERROR: failed call! ret = %d\n", ret); \
    goto _finish; \
}

void saveAsBMP(BYTE *pData, INT rowPitch, INT height, const char *fileName)
{
    BITMAPINFOHEADER bmInfo = {};
    bmInfo.biSize = sizeof(BITMAPINFOHEADER);
    bmInfo.biWidth = rowPitch / 4;
    bmInfo.biHeight = -height;
    bmInfo.biPlanes = 1;
    bmInfo.biBitCount = 32;
    bmInfo.biCompression = BI_RGB;
    bmInfo.biSizeImage = 0;
    bmInfo.biXPelsPerMeter = 0;
    bmInfo.biYPelsPerMeter = 0;
    bmInfo.biClrUsed = 0;
    bmInfo.biClrImportant = 0;

    BITMAPFILEHEADER bmfHeader = {};
    DWORD dwSizeofImage = rowPitch * height;
    DWORD dwSizeofDIB = dwSizeofImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);
    bmfHeader.bfSize = dwSizeofDIB;
    bmfHeader.bfType = 0x4D42;

    FILE *f;
    fopen_s(&f, fileName, "wb");

    size_t dwBytesWritten = 0;
    dwBytesWritten += fwrite(&bmfHeader, sizeof(BITMAPFILEHEADER), 1, f);
    dwBytesWritten += fwrite(&bmInfo, sizeof(BITMAPINFOHEADER), 1, f);
    dwBytesWritten += fwrite(pData, 1, dwSizeofImage, f);

    fclose(f);
}

int main()
{
    int ret = 0;
    CaptureDesktop* pCapture = new CaptureDesktop;
    FrameProp prop = {};

    ret = pCapture->initialize();
    CHECK_RETURN(ret);

    ret = pCapture->getFrame(&prop);
    CHECK_RETURN(ret);

    saveAsBMP(prop.pData, prop.pitch, prop.height, "capture.bmp");

_finish:
    pCapture->destroy();
    return ret;
}

