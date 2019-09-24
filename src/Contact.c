#include "Contact.h"

bool IsBlank(const char *str, const size_t size) {
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

bool IsDigitOnly(const char *str, const size_t size) {
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

bool IsFineNumber(const char *str, const size_t size) {
  if (size != 11)
    return false;
  else if (str[0] != '0' || str[1] != '1' || str[2] != '0')
    return false;
  else
    return true;
}

bool Contact_Init(Contact *c, const char *filename) {
  c->filename = filename;
  c->f = fopen(c->filename, "r");
  c->GetName = _GetName;
  c->GetPhoneNo = _GetPhoneNo;
  c->FindName = _FindName;
  c->PrintName = _PrintName;
  c->SelectPerson = _SelectPerson;
  c->ConfirmUpdate = _ConfirmUpdate;
  c->WriteNewFile = _WriteNewFile;
  c->ContactSearch = _ContactSearch;
  c->ContactInsert = _ContactInsert;
  c->ContactUpdate = _ContactUpdate;
  c->ContactDelete = _ContactDelete;
  return true;
}

void _GetName(Contact *c) {
  const char *funcName[4] = {"검색", "추가", "수정", "삭제"};
  bool loop = true;
  while (loop) {
    printf("연락처를 %s할 사람의 이름을 입력하세요: ", funcName[c->func]);

    // scanf를 이용할 경우 공백을 문자열 구분 요소로 사용하므로
    // 라인 입력을 통해 공백을 포함한 줄을 stdin으로 입력받는다.
    fgets(c->targetName, sizeof c->targetName, stdin);
    // fgets를 통해 들어온 개행 문자를 제거해준다.
    c->targetName[strlen(c->targetName) - 1] = '\0';

    // 올바른 형식의 문자열이 들어올 때까지 루프를 반복시킨다.
    if (!strlen(c->targetName))
      puts("이름 칸은 공란이 될 수 없습니다!");
    else if (IsBlank(c->targetName, strlen(c->targetName)))
      puts("이름 란에는 공백을 넣을 수 없습니다!");
    else
      loop = false;
  }
}

void _GetPhoneNo(Contact *c) {
  const char *funcName[4] = {"검색", "추가", "수정", "삭제"};
  bool loop = true;
  while (loop) {
    printf("연락처를 %s할 사람의 번호를 입력하세요: ", funcName[c->func]);

    // scanf를 이용할 경우 공백을 문자열 구분 요소로 사용하므로
    // 라인 입력을 통해 공백을 포함한 줄을 stdin으로 입력받는다.
    fgets(c->phoneNo, sizeof c->phoneNo, stdin);
    // fgets를 통해 들어온 개행 문자를 제거해준다.
    c->phoneNo[strlen(c->phoneNo) - 1] = '\0';

    // 올바른 형식의 문자열이 들어올 때까지 루프를 반복시킨다.
    if (!strlen(c->phoneNo))
      puts("번호 칸은 공란이 될 수 없습니다!");
    else if (!IsDigitOnly(c->phoneNo, strlen(c->phoneNo)))
      puts("번호 란에는 숫자 이외의 문자를 넣을 수 없습니다!");
    else if (!IsFineNumber(c->phoneNo, strlen(c->phoneNo)))
      puts("번호는 010으로 시작하는 11자리 숫자여야 합니다!");
    else
      loop = false;
  }
}

int _FindName(Contact *c) {
  int idx = 0;
  int sameLastName = 0;
  char buf[1024];

  // 찾고자 하는 이름의 성을 저장한다.
  char lastName[110];
  strncpy(lastName, c->targetName, 3);

  // 연락처 파일의 첫 줄은 카테고리를 지정하는 줄이므로,
  // 이 줄을 무시한 후 연락처를 검색해준다.
  rewind(c->f);
  fgets(buf, sizeof buf, c->f);

  // 줄을 넘기기 전에 현재 줄의 위치를 기억해둔다.
  int lineNo = 2;
  fpos_t fPos;
  fgetpos(c->f, &fPos);

  // EOF에 도달할 때까지 버퍼에 파일 내용을 한 줄씩 불러온다.
  while (fgets(buf, sizeof buf, c->f) != NULL) {
    char name[110], phone[110];
    // 버퍼의 내용을 콤마를 전후로 나누어 파싱한다.
    sscanf(buf, "%[^,],%s", name, phone);
    // 현재 줄에 저장된 사람의 성을 파싱하여 저장한다.
    char partialName[110];
    strncpy(partialName, name, 3);

    // 찾고자 하는 이름과 같은 이름이 존재할 경우,
    // 해당하는 줄의 위치를 posArr 배열에 추가한다.
    if (strcmp(name, c->targetName) == 0) {
      c->lineArr[idx].lineNo = lineNo;
      c->lineArr[idx].fpos = fPos;
      idx++;
    }
    // 찾고자 하는 이름과 같은 성씨를 지닌 사람이 존재할 경우,
    // 성씨가 같은 사람의 수를 증가시킨다.
    if (strcmp(partialName, lastName) == 0) sameLastName++;

    // 줄을 넘기기 전에 현재 줄의 위치를 기억해둔다.
    fgetpos(c->f, &fPos);
    lineNo++;
  }
  // 검색 모드일 경우, 성씨가 같은 사람의 수를 출력한다.
  if (c->func == SEARCH)
    printf("당신과 성이 같은 사람은 총 %d명입니다.\n", sameLastName);
  // 찾은 이름의 갯수를 반환한다.
  return idx;
}

void _PrintName(Contact *c, int cnt) {
  if (cnt == 0) {
    puts("찾고자 하는 이름이 연락처에 존재하지 않습니다.");
    return;
  }

  printf("총 %d건의 연락처가 검색되었습니다.\n", cnt);
  for (int i = 0; i < cnt; i++) {
    fsetpos(c->f, &(c->lineArr[i].fpos));
    char buf[1024], name[110], phone[110];
    fgets(buf, sizeof buf, c->f);
    // 버퍼의 내용을 콤마를 전후로 나누어 파싱한다.
    sscanf(buf, "%[^,],%s", name, phone);
    printf("%5d. 이름: %s / 번호: %s\n", i + 1, name, phone);
  }
}

int _SelectPerson(Contact *c, int cnt) {
  const char *funcName[4] = {"검색", "추가", "수정", "삭제"};
  int select = -1;
  bool loop = true;
  while (loop) {
    printf("%s을 원하는 연락처를 선택하세요: ", funcName[c->func]);

    // scanf를 이용할 경우 공백을 문자열 구분 요소로 사용하므로
    // 라인 입력을 통해 공백을 포함한 줄을 stdin으로 입력받는다.
    char buf[1024];
    fgets(buf, sizeof buf, stdin);
    // fgets를 통해 들어온 개행 문자를 제거해준다.
    buf[strlen(buf) - 1] = '\0';

    // 올바른 형식의 문자열이 들어올 때까지 루프를 반복시킨다.
    if (!IsDigitOnly(buf, strlen(buf)) || !strlen(buf))
      puts("수만 입력 가능합니다!");
    else {
      sscanf(buf, "%d", &select);
      if (select > cnt || select <= 0)
        puts("입력 범위를 넘어섰습니다!");
      else
        loop = false;
    }
  }
  return select - 1;
}

bool _ConfirmUpdate(Contact *c) {
  bool loop = true;
  while (loop) {
    printf("이름: %s / 번호: %s\n", c->targetName, c->phoneNo);
    if (c->func == INSERT)
      printf("해당 인물을 연락처에 추가하시겠습니까? (Y/N) ");
    if (c->func == DELETE)
      printf("해당 인물을 정말로 연락처에서 삭제하시겠습니까? (Y/N) ");
    // scanf를 이용할 경우 공백을 문자열 구분 요소로 사용하므로
    // 라인 입력을 통해 공백을 포함한 줄을 stdin으로 입력받는다.
    char buf[1024];
    fgets(buf, sizeof buf, stdin);
    // fgets를 통해 들어온 개행 문자를 제거해준다.
    buf[strlen(buf) - 1] = '\0';

    // 올바른 형식의 문자열이 들어올 때까지 루프를 반복시킨다.
    if (strlen(buf) != 1)
      puts("잘못된 형식입니다!");
    else {
      char chr = '\0';
      sscanf(buf, "%c", &chr);
      if (chr == 'Y')
        loop = false;
      else if (chr == 'N') {
        puts("취소되었습니다.");
        return false;
      } else
        puts("잘못된 형식입니다!");
    }
  }
  return true;
}

void _WriteNewFile(Contact *c, int idx) {
  // 연락처 파일 수정용 임시파일을 생성한다.
  const char tmpname[] = "tmpfile.tmp";
  FILE *tmpf = fopen(tmpname, "w");
  int lineNo = (idx == -1 ? idx : c->lineArr[idx].lineNo);

  // 현재 열려있는 연락처 파일 내의 모든 내용을 복사한다.
  rewind(c->f);
  int curLineNo = 1;
  char buf[1024];
  while (fgets(buf, sizeof buf, c->f) != NULL) {
    // 수정 / 삭제중일 경우, 갱신된 줄을 새로 작성한다.
    if (curLineNo == lineNo) {
      if (c->func == UPDATE) {
        char name[110], tmp[110];
        sscanf(buf, "%[^,],%s", name, tmp);
        fprintf(tmpf, "%s,%s\n", name, c->phoneNo);
      }
      curLineNo++;
      continue;
    }
    fputs(buf, tmpf);
    curLineNo++;
  }
  // 삽입중인 경우, 파일 최하단에 새로운 연락처를 저장한다.
  if (c->func == INSERT) fprintf(tmpf, "%s,%s\n", c->targetName, c->phoneNo);

  // 작성이 완료되면, 기존 파일을 삭제한 후 임시 파일을
  // 이름 변경하여 다시 열어준다.
  fclose(tmpf);
  fclose(c->f);
  remove(c->filename);
  rename(tmpname, c->filename);
  c->f = fopen(c->filename, "r");
}

void _ContactSearch(Contact *c) {
  c->func = SEARCH;

  // 연락처를 검색할 사람의 이름을 입력받는다.
  c->GetName(c);

  // 일치하는 이름이 있는지 찾은 후 이를 출력시킨다.
  int cnt = c->FindName(c);
  c->PrintName(c, cnt);
}

void _ContactInsert(Contact *c) {
  c->func = INSERT;

  // 연락처에 추가할 사람의 정보를 입력받는다.
  c->GetName(c);
  c->GetPhoneNo(c);

  // 추가를 진행하기 전 사용자에게 한 번 더 확인한다.
  if (!c->ConfirmUpdate(c)) return;

  // 이름과 번호를 연락처 파일의 최하단에 저장한다.
  c->WriteNewFile(c, -1);
  printf("%s님을 연락처에 성공적으로 저장하였습니다.\n", c->targetName);
}

void _ContactUpdate(Contact *c) {
  c->func = UPDATE;

  // 연락처를 수정할 사람의 이름을 입력받는다.
  c->GetName(c);

  // 일치하는 이름이 있는지 찾은 후 이를 출력시킨다.
  int cnt = c->FindName(c);
  c->PrintName(c, cnt);
  // 해당하는 인물이 없을 시 함수를 종료한다.
  if (cnt == 0) return;

  int select = 0;
  // 동일 인물이 있을 시, 수정할 사람을 고르는 메뉴를 띄운다.
  if (cnt != 1) select = c->SelectPerson(c, cnt);

  // 연락처에 저장할 전화번호를 입력받는다
  c->GetPhoneNo(c);

  // 해당 연락처를 수정한 파일을 새로 작성한다.
  c->WriteNewFile(c, select);
  printf("%s님의 번호를 성공적으로 변경하였습니다.\n", c->targetName);
}

void _ContactDelete(Contact *c) {
  c->func = DELETE;

  // 연락처를 수정할 사람의 이름을 입력받는다.
  c->GetName(c);

  // 해당하는 인물을 찾아 lineArr에 저장한다.
  // 해당하는 인물이 없을 시 함수를 종료한다.
  int cnt = c->FindName(c);
  c->PrintName(c, cnt);
  if (cnt == 0) return;

  // 동일 인물이 있을 시, 수정할 사람을 고르는 메뉴를 띄운다.
  int select = 0;
  if (cnt != 1) select = c->SelectPerson(c, cnt);

  // 콘솔에 표기할 이름을 실제 삭제할 이름으로 변경한다.
  char buf[1024];
  fsetpos(c->f, &(c->lineArr[select].fpos));
  fgets(buf, sizeof buf, c->f);
  sscanf(buf, "%[^,],%s", c->targetName, c->phoneNo);

  // 삭제를 진행하기 전 사용자에게 한 번 더 확인한다.
  if (!c->ConfirmUpdate(c)) return;

  // 해당 연락처를 삭제한 파일을 새로 작성한다.
  c->WriteNewFile(c, select);
  printf("%s님을 성공적으로 연락처에서 삭제하였습니다.\n", c->targetName);
}
