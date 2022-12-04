#include "dir.h"
#include "bitmap.h"

int main()
{
    char file_path[_MAX_PATH] = "./image_file";
    printf("파일 검사중...\n");
    File_Search(file_path);
    printf("\n[총 %d개의 파일 검색]\n변환할 파일 번호 : ", file_num);

    int num; //원하는 숫자 번호
    scanf_s("%d", &num);
    char image_path[50];
    strcpy(image_path, file_arr[num]);
    
    int retr = bitfunc(image_path);
    if (retr == 0) {
        printf("\n프로그램 실행 오류\n");
        system("PAUSE");
        return 0;
    }
    else {
        printf("\n프로그램 실행 완료\n");
        system("PAUSE");
        return 0;
    }
}
