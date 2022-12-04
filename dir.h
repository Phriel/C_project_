#pragma once
#define _CRT_SECURE_NO_WARNINGS    // fopen ���� ���� ���� ������ ���� ����
#include <stdio.h>    // fopen, fread, fseek, fprintf, fclose �Լ��� ����� ��� ����
#include <stdlib.h>   // malloc, free �Լ��� ����� ��� ����
#include <string.h>
#include <io.h>
#include <Windows.h>

int file_num = 0;
char file_arr[100][50];

struct _finddata_t fd;
int isFileOrDir()
{
    if (fd.attrib & _A_SUBDIR)
        return 0; // ���丮�� 0 ��ȯ
    else
        return 1; // �׹��� ���� "�����ϴ� ����"�̱⿡ 1 ��ȯ

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

        check = isFileOrDir();    //�������� ���丮 ���� �ĺ�

        if (check == 0 && fd.name[0] != '.')
        {
            File_Search(file_pt);    //���� ���丮 �˻� ����Լ�
        }
        else if (check == 1 && fd.size != 0 && fd.name[0] != '.')
        {
            strcpy(file_arr[file_num], file_pt);
            printf("����[%d��] : %s, ũ��:%d����Ʈ\n", file_num, file_pt, fd.size);
            file_num++;
        }
    }
    _findclose(handle);
}