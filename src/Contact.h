#ifndef CONTACT_H
#define CONTACT_H

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 문자열 내에 공백이 포함되어 있는지 확인하는 함수
bool IsBlank(const char *str, const size_t size);
// 문자열이 숫자로만 이루어져 있는지 확인하는 함수
bool IsDigitOnly(const char *str, const size_t size);
// 문자열이 올바른 전화번호의 형식인지 확인하는 함수
bool IsFineNumber(const char *str, const size_t size);

// 원하는 기능이 수정인지 삭제인지를 저장하는 열거형
// SEARCH, INSERT, UPDATE, DELETE 네 개로 구분한다
enum FUNC { SEARCH = 0, INSERT, UPDATE, DELETE };

// 원하는 연락처로 쉽게 이동하기 위해
// 해당 줄의 커서 위치와 줄 번호를 저장하는 구조체
typedef struct {
  fpos_t fpos;
  int lineNo;
} fileLine;

// 연락처 관리 구조체
typedef struct _Contact {
  // 현재 선택된 기능이 어떤 기능인지 저장하는 열거형
  enum FUNC func;
  // 입출력 파일을 저장시킬 파일 포인터
  FILE *f;

  // 입출력 도중 전달시킬 문자열을 저장할 배열
  char targetName[110];
  char phoneNo[110];

  // 입출력에 사용할 파일 이름을 저장할 포인터
  const char *filename;

  // 인원 검색에 사용할 배열
  fileLine lineArr[50000];

  // 올바른 이름 형식을 입력받아 targetName에 저장하는 함수
  void (*GetName)(struct _Contact *);
  // 올바른 전화번호 형식을 입력받아 phoneNo에 저장하는 함수
  void (*GetPhoneNo)(struct _Contact *);
  // 파일 내에서 targetName을 검색한 후, 해당하는 이름의 갯수를
  // 반환함과 동시에 이 이름이 들어있는 파일 위치를 저장하는 함수
  int (*FindName)(struct _Contact *);
  // lineArr에 저장된 연락처들을 출력하는 함수
  void (*PrintName)(struct _Contact *, int);
  // lineArr 배열에 저장된 이름 중 수정/삭제할 연락처를 선택하는 함수
  int (*SelectPerson)(struct _Contact *, int);
  // 연락처를 업데이트 하기 전 사용자에게 재확인하는 함수
  bool (*ConfirmUpdate)(struct _Contact *);
  // lineno 번째 줄이 수정된 연락처가 수정된 임시 파일을 작성한 후,
  // 이를 기존 파일과 교체해주는 함수
  void (*WriteNewFile)(struct _Contact *, int);

  // 연락처 검색을 실행하는 함수
  void (*ContactSearch)(struct _Contact *);
  // 연락처 삽입을 실행하는 함수
  void (*ContactInsert)(struct _Contact *);
  // 연락처 수정을 실행하는 함수
  void (*ContactUpdate)(struct _Contact *);
  // 연락처 삭제를 실행하는 함수
  void (*ContactDelete)(struct _Contact *);
} Contact;

// 연락처 구조체를 초기화하는 함수
bool Contact_Init(Contact *c, const char *filename);

void _GetName(Contact *c);
void _GetPhoneNo(Contact *c);
int _FindName(Contact *c);
void _PrintName(Contact *c, int);
int _SelectPerson(Contact *c, int cnt);
bool _ConfirmUpdate(Contact *c);
void _WriteNewFile(Contact *c, int lineNo);

void _ContactSearch(Contact *c);
void _ContactInsert(Contact *c);
void _ContactUpdate(Contact *c);
void _ContactDelete(Contact *c);

#endif
