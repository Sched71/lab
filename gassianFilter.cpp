#include <iostream>
#include <fstream>
#include "Header.h"
#include <io.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

int main() {
    setlocale(LC_CTYPE, "rus");
    ifstream myPicture("D://photo//sample.bmp", ifstream::binary);
    if (!myPicture) {
        std::cout << "Ошибка открытия файла. Попробуйте повторить попытку или введите другое имя.";
        myPicture.close();
        return 0;
    }

    //читаем заголовок
    BITMAPFILEHEADER myHeader;                                                      // это типо название? подсказка: myHeader{}
    myPicture.read(reinterpret_cast<char*>(&myHeader.bfType), sizeof(myHeader.bfType));   // преобразовываем адрес myHeader.bfType к типу char и читаем в переменную myHeader.bfType?(преобразовываем указ-ль любого типа в указ-ль типа char) 
    myPicture.read(reinterpret_cast<char*>(&myHeader.bfSize), sizeof(myHeader.bfSize));// читаем sizeof байтов - к-во байт к-ые занимает опр-ый тип в памяти
    myPicture.read(reinterpret_cast<char*>(&myHeader.bfReserved1), sizeof(myHeader.bfReserved1));
    myPicture.read(reinterpret_cast<char*>(&myHeader.bfReserved2), sizeof(myHeader.bfReserved2));
    myPicture.read(reinterpret_cast<char*>(&myHeader.bfOffBits), sizeof(myHeader.bfOffBits));
    //проверяем условия, связанные с заголовком
    if (myHeader.bfType != 0x4d42) {
        cout << "Данный файл не является BMP файлом." << endl;
        myPicture.close();
        return 0;
    }

    //читаем информацию
    BITMAPINFOHEADER myInfo;
    myPicture.read(reinterpret_cast<char*>(&myInfo.biSize), sizeof(myInfo.biSize));
    myPicture.read(reinterpret_cast<char*>(&myInfo.biWidth), sizeof(myInfo.biWidth));
    myPicture.read(reinterpret_cast<char*>(&myInfo.biHeight), sizeof(myInfo.biHeight));
    myPicture.read(reinterpret_cast<char*>(&myInfo.biPlanes), sizeof(myInfo.biPlanes));
    myPicture.read(reinterpret_cast<char*>(&myInfo.biBitCount), sizeof(myInfo.biBitCount));
    myPicture.read(reinterpret_cast<char*>(&myInfo.biCompression), sizeof(myInfo.biCompression));
    myPicture.read(reinterpret_cast<char*>(&myInfo.biSizeImage), sizeof(myInfo.biSizeImage));
    myPicture.read(reinterpret_cast<char*>(&myInfo.biXPelsPerMeter), sizeof(myInfo.biXPelsPerMeter));
    myPicture.read(reinterpret_cast<char*>(&myInfo.biYPelsPerMeter), sizeof(myInfo.biYPelsPerMeter));
    myPicture.read(reinterpret_cast<char*>(&myInfo.biClrUsed), sizeof(myInfo.biClrUsed));
    myPicture.read(reinterpret_cast<char*>(&myInfo.biClrImportant), sizeof(myInfo.biClrImportant));
    if (myInfo.biSize == 108 || myInfo.biSize == 124)
    {
        myPicture.read(reinterpret_cast<char*>(&myInfo.biRedMask), sizeof(myInfo.biRedMask));
        myPicture.read(reinterpret_cast<char*>(&myInfo.biGreenMask), sizeof(myInfo.biGreenMask));
        myPicture.read(reinterpret_cast<char*>(&myInfo.biBlueMask), sizeof(myInfo.biBlueMask));
        myPicture.read(reinterpret_cast<char*>(&myInfo.biAlphaMask), sizeof(myInfo.biAlphaMask));
        myPicture.read(reinterpret_cast<char*>(&myInfo.biCSType), sizeof(myInfo.biCSType));
        myPicture.read(reinterpret_cast<char*>(&myInfo.biEndpoints), sizeof(myInfo.biEndpoints));
        myPicture.read(reinterpret_cast<char*>(&myInfo.biGammaRed), sizeof(myInfo.biGammaRed));
        myPicture.read(reinterpret_cast<char*>(&myInfo.biGammaGreen), sizeof(myInfo.biGammaGreen));
        myPicture.read(reinterpret_cast<char*>(&myInfo.biGammaBlue), sizeof(myInfo.biGammaBlue));
        if (myInfo.biSize == 124) {
            myPicture.read(reinterpret_cast<char*>(&myInfo.biIntent), sizeof(myInfo.biIntent));
            myPicture.read(reinterpret_cast<char*>(&myInfo.biProfileData), sizeof(myInfo.biProfileData));
            myPicture.read(reinterpret_cast<char*>(&myInfo.biProfileSize), sizeof(myInfo.biProfileSize));
            myPicture.read(reinterpret_cast<char*>(&myInfo.biReserved), sizeof(myInfo.biReserved));
        }
    }
    //проверяем условия
    if ((myInfo.biSize != 40 && myInfo.biSize != 108 && myInfo.biSize != 124) ||
        myHeader.bfReserved1 != 0 ||
        myHeader.bfReserved2 != 0 ||
        myInfo.biPlanes != 1 ||
        myInfo.biCompression != 0 ||  //рассматриваем только несжатые изображения
        myInfo.biBitCount != 24) //рассматриваем только полноцветные изображения
    {
        cout << "Неподдерживаемый BMP формат." << endl;
        myPicture.close();
        return 0;
    }




    // Заголовок прочитан и проверен
    //Считаем ширину строки, учитывая выравнивание
    int bytesPerPixel = myInfo.biBitCount / 8;
    int alignment = 4;                         // выравнивание
    int stride = (myInfo.biWidth * bytesPerPixel) + (alignment - 1);
    stride /= alignment;
    stride *= alignment;  // ширина кратная  4
    int padding = stride - myInfo.biWidth * bytesPerPixel;  

    //создаем буфер
    char* buffer = new char[stride * myInfo.biHeight];
    //читаем в буфер
    myPicture.read(reinterpret_cast<char*> (buffer), stride * myInfo.biHeight);
    myPicture.close();


    // Создаем новый буфер для повернутого изображения
    int newPadding = ((myInfo.biHeight * bytesPerPixel + 3) / 4) * 4 - myInfo.biHeight * bytesPerPixel;
    int newStride = myInfo.biHeight * bytesPerPixel + newPadding;        // новая ширина кратная 4
    char* newBuffer = new char[myInfo.biWidth * newStride];
    char null = buffer[0];
    myHeader.bfSize = 14 + myInfo.biSize + myInfo.biWidth * newStride;// считаем новый размер файла с повернутым изображением

    BITMAPFILEHEADER myNewHeader;
    ofstream myNewPicture("D://photo//RESbmpRotateV2(GaussianFilter).bmp", ofstream::binary);
    myNewPicture.write(reinterpret_cast<char*>(&myHeader.bfType), sizeof(myHeader.bfType));
    myNewPicture.write(reinterpret_cast<char*>(&myHeader.bfSize), sizeof(myHeader.bfSize));
    myNewPicture.write(reinterpret_cast<char*>(&myHeader.bfReserved1), sizeof(myHeader.bfReserved1));
    myNewPicture.write(reinterpret_cast<char*>(&myHeader.bfReserved2), sizeof(myHeader.bfReserved2));
    myNewPicture.write(reinterpret_cast<char*>(&myHeader.bfOffBits), sizeof(myHeader.bfOffBits));
    myNewPicture.write(reinterpret_cast<char*>(&myInfo.biSize), sizeof(myInfo.biSize));
    myNewPicture.write(reinterpret_cast<char*>(&myInfo.biHeight), sizeof(myInfo.biHeight));    // поменяли местами высоту и ширину
    myNewPicture.write(reinterpret_cast<char*>(&myInfo.biWidth), sizeof(myInfo.biWidth));
    myNewPicture.write(reinterpret_cast<char*>(&myInfo.biPlanes), sizeof(myInfo.biPlanes));
    myNewPicture.write(reinterpret_cast<char*>(&myInfo.biBitCount), sizeof(myInfo.biBitCount));
    myNewPicture.write(reinterpret_cast<char*>(&myInfo.biCompression), sizeof(myInfo.biCompression));
    myNewPicture.write(reinterpret_cast<char*>(&myInfo.biSizeImage), sizeof(myInfo.biSizeImage));
    myNewPicture.write(reinterpret_cast<char*>(&myInfo.biXPelsPerMeter), sizeof(myInfo.biXPelsPerMeter));
    myNewPicture.write(reinterpret_cast<char*>(&myInfo.biYPelsPerMeter), sizeof(myInfo.biYPelsPerMeter));
    myNewPicture.write(reinterpret_cast<char*>(&myInfo.biClrUsed), sizeof(myInfo.biClrUsed));
    myNewPicture.write(reinterpret_cast<char*>(&myInfo.biClrImportant), sizeof(myInfo.biClrImportant));
    if (myInfo.biSize == 108 || myInfo.biSize == 124)
    {
        myNewPicture.write(reinterpret_cast<char*>(&myInfo.biRedMask), sizeof(myInfo.biRedMask));
        myNewPicture.write(reinterpret_cast<char*>(&myInfo.biGreenMask), sizeof(myInfo.biGreenMask));
        myNewPicture.write(reinterpret_cast<char*>(&myInfo.biBlueMask), sizeof(myInfo.biBlueMask));
        myNewPicture.write(reinterpret_cast<char*>(&myInfo.biAlphaMask), sizeof(myInfo.biAlphaMask));
        myNewPicture.write(reinterpret_cast<char*>(&myInfo.biCSType), sizeof(myInfo.biCSType));
        myNewPicture.write(reinterpret_cast<char*>(&myInfo.biEndpoints), sizeof(myInfo.biEndpoints));
        myNewPicture.write(reinterpret_cast<char*>(&myInfo.biGammaRed), sizeof(myInfo.biGammaRed));
        myNewPicture.write(reinterpret_cast<char*>(&myInfo.biGammaGreen), sizeof(myInfo.biGammaGreen));
        myNewPicture.write(reinterpret_cast<char*>(&myInfo.biGammaBlue), sizeof(myInfo.biGammaBlue));
        if (myInfo.biSize == 124) {
            myNewPicture.write(reinterpret_cast<char*>(&myInfo.biIntent), sizeof(myInfo.biIntent));
            myNewPicture.write(reinterpret_cast<char*>(&myInfo.biProfileData), sizeof(myInfo.biProfileData));
            myNewPicture.write(reinterpret_cast<char*>(&myInfo.biProfileSize), sizeof(myInfo.biProfileSize));
            myNewPicture.write(reinterpret_cast<char*>(&myInfo.biReserved), sizeof(myInfo.biReserved));
        }
    }
    // поворачиваем изображение
    for (int i = 0; i < myInfo.biHeight; i++) { 
        for (int j = 0; j < myInfo.biWidth; j++) {

            int oldIndex = i * myInfo.biWidth + j;                    // индекс без учета трех байт в пикселе и без padding
            int newIndex = (myInfo.biHeight - i - 1) + j * myInfo.biHeight;

            oldIndex = oldIndex * bytesPerPixel + i * padding;      // теперь старый индекс соответствует индексу buffer
            newIndex = newIndex * bytesPerPixel + (newIndex / myInfo.biHeight) * newPadding; // теперь новый индекс соответствует индексу newBuffer

            for (int k = 0; k < bytesPerPixel; k++) {
                newBuffer[newIndex + k] = buffer[oldIndex + k];      //заполняем соответственно
            }
        }
    }
    delete[] buffer; // удаляем буфер исходного изображения    
    


    //фильтр Гаусса
    // создаем ядро
    const int radius = 1;                  // = 1
    double sigma = 1.92;                   // = 1.92
    const int kernelSide = radius * 2 + 1; // сторона ядра
    double kernel[kernelSide * kernelSide];
    double sum = 0;

    for (int i = -radius; i <= radius; i++) {
        for (int j = -radius; j <= radius; j++) {

            kernel[(i + radius) * kernelSide + (j + radius)] = exp(-(i * i + j * j) / (2 * sigma * sigma));
            sum += kernel[(i + radius) * kernelSide + (j + radius)];
        }
    }

    // нормализуем коэффициенты
    for (int i = 0; i < kernelSide * kernelSide; i++) {
        kernel[i] /= sum;
    }

    // создаем новый буфер для фильтра гаусса
    char* newBufferGauss = new char[myInfo.biWidth * newStride];

    // создали ядро, теперь 
    for (int i = 0; i < myInfo.biWidth; i++) {                // номер строки (т. к. изображение повернуто)
        for (int j = 0; j < myInfo.biHeight; j++) {           // номер столбца изображения
            double byte1 = 0;
            double byte2 = 0;
            double byte3 = 0;

            for (int x = -radius; x <= radius; x++) {         // номер строки
                for (int y = -radius; y <= radius; y++) {     // номер столбца

                    if ((i + x >= 0 + radius) && (i + x <= myInfo.biWidth - radius) && (j + y >= 0 + radius) && (j + y <= myInfo.biHeight - radius)) {
                        int kernelIndex = (x + radius) * kernelSide + (y + radius);
                        int newBufferIndex = bytesPerPixel * ((i + x) * myInfo.biHeight + (j + y)) + (i + x) * newPadding;

                        byte1 += kernel[kernelIndex] * newBuffer[newBufferIndex];
                        byte2 += kernel[kernelIndex] * newBuffer[newBufferIndex + 1];
                        byte3 += kernel[kernelIndex] * newBuffer[newBufferIndex + 2];
                    }
                }
            }
            newBufferGauss[bytesPerPixel * (i * myInfo.biHeight + j) + i * newPadding] = (unsigned char) byte1;
            newBufferGauss[bytesPerPixel * (i * myInfo.biHeight + j) + i * newPadding + 1] = (unsigned char) byte2;
            newBufferGauss[bytesPerPixel * (i * myInfo.biHeight + j) + i * newPadding + 2] = (unsigned char) byte3;
        }
    }

    myNewPicture.write(reinterpret_cast<char*> (newBufferGauss), newStride * myInfo.biWidth);
    myNewPicture.close();
    
    delete[] newBuffer;
    delete[] newBufferGauss;


    return 0;
}