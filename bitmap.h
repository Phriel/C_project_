#pragma once

#pragma pack(push,1)                // 구조체를 1바이트 크기로 정렬
typedef struct {      // BMP 비트맵 파일 헤더 구조체
    unsigned short bfType;           // BMP 파일 매직 넘버
    unsigned int   bfSize;           // 파일 크기
    unsigned short bfReserved1;      // 예약
    unsigned short bfReserved2;      // 예약
    unsigned int   bfOffBits;        // 비트맵 데이터의 시작 위치
}BITMAP_FILEHEADER;
typedef struct {      // BMP 비트맵 정보 헤더 구조체(DIB 헤더)
    unsigned int   biSize;           // 현재 구조체의 크기
    int            biWidth;          // 비트맵 이미지의 가로 크기
    int            biHeight;         // 비트맵 이미지의 세로 크기
    unsigned short biPlanes;         // 사용하는 색상판의 수
    unsigned short biBitCount;       // 픽셀 하나를 표현하는 비트 수
    unsigned int   biCompression;    // 압축 방식
    unsigned int   biSizeImage;      // 비트맵 이미지의 픽셀 데이터 크기
    int            biXPelsPerMeter;  // 그림의 가로 해상도(미터당 픽셀)
    int            biYPelsPerMeter;  // 그림의 세로 해상도(미터당 픽셀)
    unsigned int   biClrUsed;        // 색상 테이블에서 실제 사용되는 색상 수
    unsigned int   biClrImportant;   // 비트맵을 표현하기 위해 필요한 색상 인덱스 수
}BITMAP_INFOHEADER;
typedef struct {            // 24비트 비트맵 이미지의 픽셀 구조체
    unsigned char rgbtBlue;          // 파랑
    unsigned char rgbtGreen;         // 초록
    unsigned char rgbtRed;           // 빨강
}RGB_TRIPLE;
#pragma pack(pop)

#define PIXEL_SIZE   3    // 픽셀 한 개의 크기 3바이트(24비트)
#define PIXEL_ALIGN  4    // 픽셀 데이터 가로 한 줄은 4의 배수 크기로 저장됨

int bitfunc(char pth[]) {
    FILE* fpBmp;                    // 비트맵 파일 포인터
    FILE* fpTxt;                    // 텍스트 파일 포인터
    BITMAP_FILEHEADER fileHeader ;    // 비트맵 파일 헤더 구조체 변수
    BITMAP_INFOHEADER infoHeader ;    // 비트맵 정보 헤더 구조체 변수

    unsigned char* image;    // 픽셀 데이터 포인터
    int size;                // 픽셀 데이터 크기
    int width, height;       // 비트맵 이미지의 가로, 세로 크기
    int padding;             // 픽셀 데이터의 가로 크기가 4의 배수가 아닐 때 남는 공간의 크기

    // 각 픽셀을 표현할 ASCII 문자. 인덱스가 높을 수록 밝아지는 것을 표현
    char ascii[] = { '#', '#', '@', '%', '=', '+', '*', ':', '-', '.', ' ' };   // 11개

    fpBmp = fopen(pth, "rb");    // 비트맵 파일을 바이너리 모드로 열기
    if (fpBmp == NULL) {    // 파일 열기에 실패하면
        printf("\n이미지파일 열기 실패\n");
        return 0;// 프로그램 종료
    }
    else if (fread(&fileHeader, sizeof(BITMAP_FILEHEADER), 1, fpBmp) < 1) // 비트맵 파일 헤더 읽기
    {
        printf("\n파일헤더 읽기 실패\n");
        fclose(fpBmp);
        return 0;
    }//읽기에 실패하면 파일 포인터를 닫고 프로그램 종료
    else if (fread(&infoHeader, sizeof(BITMAP_INFOHEADER), 1, fpBmp) < 1) // 비트맵 정보 헤더 읽기
    {
        printf("\n정보헤더 읽기 실패\n");
        fclose(fpBmp);
        return 0;
    }//읽기에 실패하면 파일 포인터를 닫고 프로그램 종료
    else if (fileHeader.bfType != 'MB') // 매직 넘버가 MB가 맞는지 확인
    {
        printf("매직넘버 오류");
        fclose(fpBmp);
        return 0;
    }// 매직 넘버가 맞지 않으면 프로그램 종료
    else if (infoHeader.biBitCount != 24)
    {
        printf("\n이미지파일이 24비트가 아님\n");
        fclose(fpBmp);
        return 0;
    }// 24비트 비트맵이 아니면 프로그램 종료

    size = infoHeader.biSizeImage;    // 픽셀 데이터 크기
    width = infoHeader.biWidth;       // 비트맵 이미지의 가로 크기
    height = infoHeader.biHeight;     // 비트맵 이미지의 세로 크기

    // 이미지의 가로 크기에 픽셀 크기를 곱하여 가로 한 줄의 크기를 구하고 4로 나머지를 구함
    padding = (PIXEL_ALIGN - ((width * PIXEL_SIZE) % PIXEL_ALIGN)) % PIXEL_ALIGN;
    
    if (size == 0)    // 픽셀 데이터 크기가 0이라면
    {
        size = (width * PIXEL_SIZE + padding) * height; // 이미지 데이터 크기 계산
    }
    image = malloc(size);    // 픽셀 데이터의 크기만큼 동적 메모리 할당
    fseek(fpBmp, fileHeader.bfOffBits, SEEK_SET); // 파일 포인터를 픽셀 데이터의 시작 위치로 이동
    if (fread(image, size, 1, fpBmp) < 1)
    {
        printf("\n 이미지 데이터 읽기 실패\n");
        fclose(fpBmp);
        return 0;
    }// 파일에서 픽셀 데이터 크기만큼 읽음. 읽기에 실패하면 파일 포인터를 닫고 프로그램 종료
    fclose(fpBmp);    // 비트맵 파일 닫기
    fpTxt = fopen("./text_file/ascii.txt", "w");    // 결과 출력용 텍스트 파일 열기
    if (fpTxt == NULL)    // 파일 열기에 실패하면
    {
        printf("\n텍스트파일 열기 실패\n");
        free(image);      // 픽셀 데이터를 저장한 동적 메모리 해제
        return 0;         // 프로그램 종료
    }
    // 픽셀 데이터는 아래 위가 뒤집혀서 저장되므로 아래쪽부터 반복, 세로크기만큼 반복
    for (int y = height - 1; y >= 0; y--)
    {
        // 가로 크기만큼 반복
        for (int x = 0; x < width; x++)
        {
            int index = (x * PIXEL_SIZE) + (y * (width * PIXEL_SIZE)) + (padding * y);//인덱스를 계산

            // 현재 픽셀의 주소를 RGBTRIPLE 포인터로 변환하여 RGBTRIPLE 포인터에 저장
            RGB_TRIPLE* pixel = (RGB_TRIPLE*)&image[index];

            // RGBTRIPLE 구조체로 파랑, 초록, 빨강값을 가져오기
            unsigned char blue = pixel->rgbtBlue;
            unsigned char green = pixel->rgbtGreen;
            unsigned char red = pixel->rgbtRed;

            // 파랑, 초록, 빨강값의 평균을 구하면 흑백 이미지를 얻을 수 있음
            unsigned char gray = (red + green + blue) / PIXEL_SIZE;

            // 흑백값에 ASCII 문자의 개수를 곱한 뒤 256으로 나누고 흑백값에 따라 아스키 인덱스 구하기
            char c = ascii[gray * sizeof(ascii) / 256];

            //이미지와 같은 형상을 위해 같은문자 두번 저장
            fprintf(fpTxt, "%c%c", c, c);    // 텍스트 파일에 문자 출력
        }

        fprintf(fpTxt, "\n");    // 가로 픽셀 저장이 끝났으면 줄바꾸기
    }

    fclose(fpTxt);    // 텍스트 파일 닫기
    free(image);      // 픽셀 데이터를 저장한 동적 메모리 해제
    return 1;
}
