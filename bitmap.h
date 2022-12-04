#pragma once

#pragma pack(push,1)                // ����ü�� 1����Ʈ ũ��� ����
typedef struct {      // BMP ��Ʈ�� ���� ��� ����ü
    unsigned short bfType;           // BMP ���� ���� �ѹ�
    unsigned int   bfSize;           // ���� ũ��
    unsigned short bfReserved1;      // ����
    unsigned short bfReserved2;      // ����
    unsigned int   bfOffBits;        // ��Ʈ�� �������� ���� ��ġ
}BITMAP_FILEHEADER;
typedef struct {      // BMP ��Ʈ�� ���� ��� ����ü(DIB ���)
    unsigned int   biSize;           // ���� ����ü�� ũ��
    int            biWidth;          // ��Ʈ�� �̹����� ���� ũ��
    int            biHeight;         // ��Ʈ�� �̹����� ���� ũ��
    unsigned short biPlanes;         // ����ϴ� �������� ��
    unsigned short biBitCount;       // �ȼ� �ϳ��� ǥ���ϴ� ��Ʈ ��
    unsigned int   biCompression;    // ���� ���
    unsigned int   biSizeImage;      // ��Ʈ�� �̹����� �ȼ� ������ ũ��
    int            biXPelsPerMeter;  // �׸��� ���� �ػ�(���ʹ� �ȼ�)
    int            biYPelsPerMeter;  // �׸��� ���� �ػ�(���ʹ� �ȼ�)
    unsigned int   biClrUsed;        // ���� ���̺��� ���� ���Ǵ� ���� ��
    unsigned int   biClrImportant;   // ��Ʈ���� ǥ���ϱ� ���� �ʿ��� ���� �ε��� ��
}BITMAP_INFOHEADER;
typedef struct {            // 24��Ʈ ��Ʈ�� �̹����� �ȼ� ����ü
    unsigned char rgbtBlue;          // �Ķ�
    unsigned char rgbtGreen;         // �ʷ�
    unsigned char rgbtRed;           // ����
}RGB_TRIPLE;
#pragma pack(pop)

#define PIXEL_SIZE   3    // �ȼ� �� ���� ũ�� 3����Ʈ(24��Ʈ)
#define PIXEL_ALIGN  4    // �ȼ� ������ ���� �� ���� 4�� ��� ũ��� �����

int bitfunc(char pth[]) {
    FILE* fpBmp;                    // ��Ʈ�� ���� ������
    FILE* fpTxt;                    // �ؽ�Ʈ ���� ������
    BITMAP_FILEHEADER fileHeader ;    // ��Ʈ�� ���� ��� ����ü ����
    BITMAP_INFOHEADER infoHeader ;    // ��Ʈ�� ���� ��� ����ü ����

    unsigned char* image;    // �ȼ� ������ ������
    int size;                // �ȼ� ������ ũ��
    int width, height;       // ��Ʈ�� �̹����� ����, ���� ũ��
    int padding;             // �ȼ� �������� ���� ũ�Ⱑ 4�� ����� �ƴ� �� ���� ������ ũ��

    // �� �ȼ��� ǥ���� ASCII ����. �ε����� ���� ���� ������� ���� ǥ��
    char ascii[] = { '#', '#', '@', '%', '=', '+', '*', ':', '-', '.', ' ' };   // 11��

    fpBmp = fopen(pth, "rb");    // ��Ʈ�� ������ ���̳ʸ� ���� ����
    if (fpBmp == NULL) {    // ���� ���⿡ �����ϸ�
        printf("\n�̹������� ���� ����\n");
        return 0;// ���α׷� ����
    }
    else if (fread(&fileHeader, sizeof(BITMAP_FILEHEADER), 1, fpBmp) < 1) // ��Ʈ�� ���� ��� �б�
    {
        printf("\n������� �б� ����\n");
        fclose(fpBmp);
        return 0;
    }//�б⿡ �����ϸ� ���� �����͸� �ݰ� ���α׷� ����
    else if (fread(&infoHeader, sizeof(BITMAP_INFOHEADER), 1, fpBmp) < 1) // ��Ʈ�� ���� ��� �б�
    {
        printf("\n������� �б� ����\n");
        fclose(fpBmp);
        return 0;
    }//�б⿡ �����ϸ� ���� �����͸� �ݰ� ���α׷� ����
    else if (fileHeader.bfType != 'MB') // ���� �ѹ��� MB�� �´��� Ȯ��
    {
        printf("�����ѹ� ����");
        fclose(fpBmp);
        return 0;
    }// ���� �ѹ��� ���� ������ ���α׷� ����
    else if (infoHeader.biBitCount != 24)
    {
        printf("\n�̹��������� 24��Ʈ�� �ƴ�\n");
        fclose(fpBmp);
        return 0;
    }// 24��Ʈ ��Ʈ���� �ƴϸ� ���α׷� ����

    size = infoHeader.biSizeImage;    // �ȼ� ������ ũ��
    width = infoHeader.biWidth;       // ��Ʈ�� �̹����� ���� ũ��
    height = infoHeader.biHeight;     // ��Ʈ�� �̹����� ���� ũ��

    // �̹����� ���� ũ�⿡ �ȼ� ũ�⸦ ���Ͽ� ���� �� ���� ũ�⸦ ���ϰ� 4�� �������� ����
    padding = (PIXEL_ALIGN - ((width * PIXEL_SIZE) % PIXEL_ALIGN)) % PIXEL_ALIGN;
    
    if (size == 0)    // �ȼ� ������ ũ�Ⱑ 0�̶��
    {
        size = (width * PIXEL_SIZE + padding) * height; // �̹��� ������ ũ�� ���
    }
    image = malloc(size);    // �ȼ� �������� ũ�⸸ŭ ���� �޸� �Ҵ�
    fseek(fpBmp, fileHeader.bfOffBits, SEEK_SET); // ���� �����͸� �ȼ� �������� ���� ��ġ�� �̵�
    if (fread(image, size, 1, fpBmp) < 1)
    {
        printf("\n �̹��� ������ �б� ����\n");
        fclose(fpBmp);
        //return 0;
    }// ���Ͽ��� �ȼ� ������ ũ�⸸ŭ ����. �б⿡ �����ϸ� ���� �����͸� �ݰ� ���α׷� ����
    fclose(fpBmp);    // ��Ʈ�� ���� �ݱ�
    fpTxt = fopen("./text_file/ascii.txt", "w");    // ��� ��¿� �ؽ�Ʈ ���� ����
    if (fpTxt == NULL)    // ���� ���⿡ �����ϸ�
    {
        printf("\n�ؽ�Ʈ���� ���� ����\n");
        free(image);      // �ȼ� �����͸� ������ ���� �޸� ����
        //return 0;         // ���α׷� ����
    }
    // �ȼ� �����ʹ� �Ʒ� ���� �������� ����ǹǷ� �Ʒ��ʺ��� �ݺ�, ����ũ�⸸ŭ �ݺ�
    for (int y = height - 1; y >= 0; y--)
    {
        // ���� ũ�⸸ŭ �ݺ�
        for (int x = 0; x < width; x++)
        {
            int index = (x * PIXEL_SIZE) + (y * (width * PIXEL_SIZE)) + (padding * y);//�ε����� ���

            // ���� �ȼ��� �ּҸ� RGBTRIPLE �����ͷ� ��ȯ�Ͽ� RGBTRIPLE �����Ϳ� ����
            RGB_TRIPLE* pixel = (RGB_TRIPLE*)&image[index];

            // RGBTRIPLE ����ü�� �Ķ�, �ʷ�, �������� ��������
            unsigned char blue = pixel->rgbtBlue;
            unsigned char green = pixel->rgbtGreen;
            unsigned char red = pixel->rgbtRed;

            // �Ķ�, �ʷ�, �������� ����� ���ϸ� ��� �̹����� ���� �� ����
            unsigned char gray = (red + green + blue) / PIXEL_SIZE;

            // ��鰪�� ASCII ������ ������ ���� �� 256���� ������ ��鰪�� ���� �ƽ�Ű �ε��� ���ϱ�
            char c = ascii[gray * sizeof(ascii) / 256];

            //�̹����� ���� ������ ���� �������� �ι� ����
            fprintf(fpTxt, "%c%c", c, c);    // �ؽ�Ʈ ���Ͽ� ���� ���
        }

        fprintf(fpTxt, "\n");    // ���� �ȼ� ������ �������� �ٹٲٱ�
    }

    fclose(fpTxt);    // �ؽ�Ʈ ���� �ݱ�
    free(image);      // �ȼ� �����͸� ������ ���� �޸� ����
    return 1;
}