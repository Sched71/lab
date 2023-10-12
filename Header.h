#pragma once
#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED
typedef int FXPT2DOT30; 

typedef struct {        
    FXPT2DOT30 ciexyzX;
    FXPT2DOT30 ciexyzY;
    FXPT2DOT30 ciexyzZ;
} CIEXYZ;

typedef struct {
    CIEXYZ  ciexyzRed;
    CIEXYZ  ciexyzGreen;
    CIEXYZ  ciexyzBlue;
} CIEXYZTRIPLE;

typedef struct {
    unsigned short  bfType; //сигнатура формата файла (формат файла), должно быть BM = 0x4d42
    unsigned int   bfSize; //размер файла в байтах
    unsigned short    bfReserved1; //2 байта зарезервированы и должны быть установлены на 0
    unsigned short    bfReserved2; //см. bfReserved1
    unsigned int   bfOffBits; //смещение от начала до растровых данных
} BITMAPFILEHEADER;

typedef struct
{
    unsigned int    biSize; //размер данной структуры в байтах, указывающий также на на версию структуры
    int             biWidth; //ширина растра в пикселях. указывается целым числом со знаком. ноль и отрицательные не документированы
    int             biHeight; //целое число со знаком, содержащее 2 параметра: высота растра в пикселях (абсолютное значение числа) и порядок следования строк в двумерных массивах (знак числа). нулевое значение не документировано.
    unsigned short  biPlanes; //2 байта, указывающие количество цветовых плоскостей для целевого устройста, всегда равное 1
    unsigned short  biBitCount; //количество бит на пиксель, равно 0 (пиксели хранятся в формате JPEG или PNG), 1, 4, 8, 16, 24, 32. У нас 24
    unsigned int    biCompression; //указывает на способ хранения пикселей. BI_RGB | BI_RLE8 | BI_RLE4 |BI_BITFIELDS | BI_JPEG | BI_PNG. реально используется только BI_RGB. Указывает тип сжатия изображения, BI_RGB означает отсутствие сжатия (это поле равно 0)
    unsigned int    biSizeImage; //указывает размер растровых данных; при использовании BI_RGB его можно установить равным 0 (т.е. если хранение осуществляется двумерным массивом)
    int             biXPelsPerMeter; //указывает горизонтальное разрешение, единица измерения - пиксель/метр, целое число со знаком
    int             biYPelsPerMeter; //указывает вертикальное разрешение, единица измерения - пиксель/метр, целое число со знаком
    unsigned int    biClrUsed; //количество используемых цветов (если есть таблица цветов). 0 вроде означает использовать все. Вроде для 24 RGBTRIPPLE
    unsigned int    biClrImportant; //количество важных цветов. Если 0, то все цвета считаются важными. Дальше может идти цветовая палитра
    unsigned int    biRedMask;
    unsigned int    biGreenMask;
    unsigned int    biBlueMask;
    unsigned int    biAlphaMask;
    unsigned int    biCSType;
    CIEXYZTRIPLE    biEndpoints;
    unsigned int    biGammaRed;
    unsigned int    biGammaGreen;
    unsigned int    biGammaBlue;
    unsigned int    biIntent;
    unsigned int    biProfileData;
    unsigned int    biProfileSize;
    unsigned int    biReserved;
} BITMAPINFOHEADER;

#endif // MAIN_H_INCLUDED
#pragma once