#include "ContactSearch.h"

bool IsBlank(char *str, size_t size) {
  int i = 0;
  while (i < size) {
    char chr = *(str + i);
    // 이름 문자열 내에 ASCII 이외의 문자가 있을 경우,
    // 한글의 바이트 크기인 3바이트만큼 다음 문자로 이동한다.
    if (chr & 0x80) i += 3;
    // 이름 문자열 내에 공백이 있을 경우, 거짓을 반환
    else if (isblank(chr))
      return true;
    else
      i++;
  }
  return false;
}

bool IsDigitOnly(char *str, size_t size) {
  for (int i = 0; i < size; i++) {
    char chr = *(str + i);
    // 번호 문자열 내에 ASCII 이외의 문자가 있을 경우 거짓을 반환
    if (chr & 0x80) return false;
    // 번호 문자열 내에 숫자가 아닌 ASCII 문자가 있을 경우 거짓을 반환
    else if (!isdigit(chr))
      return false;
  }
  return true;
}

int FindName(FILE *f, char targetName[], fpos_t *posArr) {
  int idx = 0;
  char buf[1024];

  // 연락처 파일의 첫 줄은 카테고리를 지정하는 줄이므로,
  // 이 줄을 무시한 후 연락처를 검색해준다.
  rewind(f);
  fgets(buf, sizeof buf, f);

  // 줄을 넘기기 전에 현재 줄의 위치를 기억해둔다.
  fpos_t fPos;
  fgetpos(f, &fPos);

  // EOF에 도달할 때까지 버퍼에 파일 내용을 한 줄씩 불러온다.
  while (fgets(buf, sizeof buf, f) != NULL) {
    char name[110], phone[110];
    // 버퍼의 내용을 콤마를 전후로 나누어 파싱한다.
    sscanf(buf, "%[^,],%s", name, phone);

    // 찾고자 하는 이름과 같은 이름이 존재할 경우,
    // 해당하는 줄의 위치를 posArr 배열에 추가한다.
    if (strcmp(targetName, name) == 0) posArr[idx++] = fPos;

    // 줄을 넘기기 전에 현재 줄의 위치를 기억해둔다.
    fgetpos(f, &fPos);
  }
  // 찾은 이름의 갯수를 반환한다.
  return idx;
}

void ContactSearch(FILE *f) {
  char targetName[110];
  bool loop = true;
  while (loop) {
    printf("찾고자 하는 사람의 이름을 입력하세요: ");

    // scanf를 이용할 경우 공백을 문자열 구분 요소로 사용하므로
    // 라인 입력을 통해 공백을 포함한 줄을 stdin으로 입력받는다.
    fgets(targetName, sizeof targetName, stdin);
    // fgets를 통해 들어온 개행 문자를 제거해준다.
    targetName[strlen(targetName) - 1] = '\0';

    // 올바른 형식의 문자열이 들어올 때까지 루프를 반복시킨다.
    if (IsBlank(targetName, strlen(targetName)))
      puts("이름 란에는 공백을 넣을 수 없습니다!");
    else
      loop = false;
  }

  fpos_t posArr[50000];
  int cnt = FindName(f, targetName, posArr);
  if (!cnt)
    puts("찾고자 하는 이름이 연락처에 존재하지 않습니다.");
  else {
    for (int i = 0; i < cnt; i++) {
      fsetpos(f, posArr + i);
      char buf[1024], name[110], phone[110];
      fgets(buf, sizeof buf, f);
      // 버퍼의 내용을 콤마를 전후로 나누어 파싱한다.
      sscanf(buf, "%[^,],%s", name, phone);
      printf("이름: %s / 번호: %s\n", name, phone);
    }
  }
}