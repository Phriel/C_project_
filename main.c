#include "dir.h"
#include "bitmap.h"
#include "head.h"

int main()
{
    char file_path[_MAX_PATH] = "./image_file";
    printf("���� �˻���...\n");
    File_Search(file_path);
    printf("\n[�� %d���� ���� �˻�]\n��ȯ�� ���� ��ȣ : ", file_num);

    int num; //���ϴ� ���� ��ȣ
    scanf_s("%d", &num);
    char image_path[50];
    strcpy(image_path, file_arr[num]);
    
    int retr = bitfunc(image_path);
    if (retr == 0) {
        printf("\n���α׷� ���� ����\n");
        system("PAUSE");
        return 0;
    }
    else {
        printf("\n���α׷� ���� �Ϸ�\n");
        system("PAUSE");
        return 0;
    }
}
