#ifndef CONTACTSEARCH_H
#define CONTACTSEARCH_H

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// 문자열 내에 공백이 포함되어 있는지 확인하는 함수
bool IsBlank(char *str, size_t size);

// 문자열이 숫자로만 이루어져 있는지 확인하는 함수
bool IsDigitOnly(char *str, size_t size);

// 파일 내에서 targetName을 검색한 후, 해당하는 이름의 갯수를
// 반환함과 동시에 이 이름이 들어있는 파일 위치를 저장하는 함수
int FindName(FILE *f, char targetName[], fpos_t *posArr);

// 연락처를 검색하는 함수
void ContactSearch(FILE *f);

#endif