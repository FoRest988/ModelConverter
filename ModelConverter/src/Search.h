#pragma once
#include <Windows.h>

typedef void (*LPSEARCHFUNC)(LPCTSTR lpszFileName);
BOOL SearchFiles(LPCTSTR lpszFileName, LPSEARCHFUNC lpSearchFunc, BOOL bInnerFolders = TRUE);


BOOL SearchFiles(LPCTSTR lpszFileName, LPSEARCHFUNC lpSearchFunc, BOOL bInnerFolders)
{
    LPTSTR part;
    char tmp[MAX_PATH]; // временны?массив
    char name[MAX_PATH];

    HANDLE hSearch = NULL;
    WIN32_FIND_DATA wfd;
    memset(&wfd, 0, sizeof(WIN32_FIND_DATA));

    // сначал?поис?внутри вложенны?папках ...

        if (GetFullPathName(lpszFileName, MAX_PATH, tmp, &part) == 0) return FALSE;
        strcpy(name, part);
        strcpy(part, "*.*");

        wfd.dwFileAttributes = FILE_ATTRIBUTE_DIRECTORY;
        if (!((hSearch = FindFirstFile(tmp, &wfd)) == INVALID_HANDLE_VALUE))
            do
            {
                // ?каждой папк?есть дв?папк??именам?"." ?".."
                // ?эт?папк?мы не трогае?
                // пропускаем папк?"." ?".."
                if (!strncmp(wfd.cFileName, ".", 1) || !strncmp(wfd.cFileName, "..", 2))
                    continue;

                if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) // если мы нашл?папк?            {
                {
                    char next[MAX_PATH];
                    if (GetFullPathName(lpszFileName, MAX_PATH, next, &part) == 0) return FALSE;
                    strcpy(part, wfd.cFileName);
                    strcat(next, "\\");
                    strcat(next, name);

                    SearchFiles(next, lpSearchFunc, TRUE);
                }
            }
    

      while (FindNextFile(hSearch, &wfd)); // ищем следующи?файл

        FindClose (hSearch); // заканчивае?поис?    }

    if ((hSearch = FindFirstFile(lpszFileName, &wfd)) == INVALID_HANDLE_VALUE) 
        return TRUE; // ?противно?случае выходи?    do
    if (!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) // если мы нашл?файл
    {
        char file[MAX_PATH];
        if(GetFullPathName(lpszFileName, MAX_PATH, file, &part) == 0) return FALSE;
        strcpy(part, wfd.cFileName);

        lpSearchFunc(file);
    }
    while (FindNextFile(hSearch, &wfd)); // ищем следующи?файл
    FindClose (hSearch); // заканчивае?поис?
    return TRUE;
}