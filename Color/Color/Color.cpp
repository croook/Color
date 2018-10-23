#include "stdafx.h"
#include <iostream>
using namespace std;
#include "./gdal/gdal_priv.h"
#pragma comment(lib, "gdal_i.lib")

int main()
{
	GDALDataset* poSrcDS;
	GDALDataset* poDstDS;
	int imgXlen, imgYlen;
	int StartX,StartY, tmpXlen, tmpYlen;
	char* srcPath = "pic.jpg";
	char* dstPath = "pic1.tif";
	GByte* buffTmp,*buff;
	int i, bandNum,j;

	GDALAllRegister();

	poSrcDS = (GDALDataset*)GDALOpenShared(srcPath, GA_ReadOnly);

	imgXlen = poSrcDS->GetRasterXSize();
	imgYlen = poSrcDS->GetRasterYSize();
	bandNum = poSrcDS->GetRasterCount();

	buffTmp = (GByte*)CPLMalloc(imgXlen * imgYlen * sizeof(GByte));


	poDstDS = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(
		dstPath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL
	);

	StartX = 100;
	StartY = 100;
	tmpXlen = 200;
	tmpYlen = 150;

	buff = (GByte*)CPLMalloc(tmpXlen*tmpYlen * sizeof(GByte));
	poSrcDS->GetRasterBand(1)->RasterIO(
		GF_Read, StartX, StartY, tmpXlen, tmpYlen, buff, tmpXlen, tmpYlen, GDT_Byte, 0, 0
	);

	for (j = 0; j < tmpYlen; j++) {
		for (i = 0; i < tmpXlen; i++) {
			buff[j*tmpXlen + i] = (GByte)255;
		}
	}


	for (i = 0; i < bandNum; i++) {
		poSrcDS->GetRasterBand(i + 1)->RasterIO(
			GF_Read, 0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0
		);
		poDstDS->GetRasterBand(i + 1)->RasterIO(
			GF_Write, 0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0
		);
		printf("... ... band %d processing ... ...\n", i);
	}

	poDstDS->GetRasterBand(1)->RasterIO(
		GF_Write, StartX, StartY, tmpXlen, tmpYlen, buff, tmpXlen, tmpYlen, GDT_Byte, 0, 0
	);


	CPLFree(buffTmp);

	GDALClose(poDstDS);
	GDALClose(poSrcDS);

	system("PAUSE");
	return 0;
}

