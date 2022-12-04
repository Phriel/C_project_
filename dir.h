#pragma once
#define _CRT_SECURE_NO_WARNINGS    // fopen 보안 경고로 인한 컴파일 에러 방지
#include <stdio.h>    // fopen, fread, fseek, fprintf, fclose 함수가 선언된 헤더 파일
#include <stdlib.h>   // malloc, free 함수가 선언된 헤더 파일
#include <string.h>
#include <io.h>
#include <Windows.h>

int file_num = 0;
char file_arr[100][50];

struct _finddata_t fd;
int isFileOrDir()
{
    if (fd.attrib & _A_SUBDIR)
        return 0; // 디렉토리면 0 반환
    else
        return 1; // 그밖의 경우는 "존재하는 파일"이기에 1 반환

}
void File_Search(char file_path[])
{
    intptr_t handle;
    int check = 0;

    char file_path2[_MAX_PATH];

    strcat(file_path, "\\");
    strcpy(file_path2, file_path);
    strcat(file_path, "*");

    if ((handle = _findfirst(file_path, &fd)) == -1)
    {
        printf("No such file or directory\n");
        return;
    }
    while (_findnext(handle, &fd) == 0)
    {

        char file_pt[_MAX_PATH];
        strcpy(file_pt, file_path2);
        strcat(file_pt, fd.name);

        check = isFileOrDir();    //파일인지 디렉토리 인지 식별

        if (check == 0 && fd.name[0] != '.')
        {
            File_Search(file_pt);    //하위 디렉토리 검색 재귀함수
        }
        else if (check == 1 && fd.size != 0 && fd.name[0] != '.')
        {
            strcpy(file_arr[file_num], file_pt);
            printf("파일[%d번] : %s, 크기:%d바이트\n", file_num, file_pt, fd.size);
            file_num++;
        }
    }
    _findclose(handle);
}