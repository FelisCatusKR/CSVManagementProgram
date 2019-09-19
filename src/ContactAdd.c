#include "ContactAdd.h"

void ContactAdd(FILE *f) {
  char name[110], phone[110];
  puts("연락처에 추가할 사람의 정보를 입력하세요.");
  bool loop = true;
  while (loop) {
    printf("이름: ");

    // scanf를 이용할 경우 공백을 문자열 구분 요소로 사용하므로
    // 라인 입력을 통해 공백을 포함한 줄을 stdin으로 입력받는다.
    fgets(name, sizeof name, stdin);
    // fgets를 통해 들어온 개행 문자를 제거해준다.
    name[strlen(name) - 1] = '\0';

    // 올바른 형식의 문자열이 들어올 때까지 루프를 반복시킨다.
    if (IsBlank(name, strlen(name)))
      puts("이름 란에는 공백을 넣을 수 없습니다!");
    else
      loop = false;
  }
  loop = true;
  while (loop) {
    printf("번호: ");

    // scanf를 이용할 경우 공백을 문자열 구분 요소로 사용하므로
    // 라인 입력을 통해 공백을 포함한 줄을 stdin으로 입력받는다.
    fgets(phone, sizeof phone, stdin);
    // fgets를 통해 들어온 개행 문자를 제거해준다.
    phone[strlen(phone) - 1] = '\0';

    // 올바른 형식의 문자열이 들어올 때까지 루프를 반복시킨다.
    if (!IsDigitOnly(phone, strlen(phone)))
      puts("번호 란에는 숫자 이외의 문자를 넣을 수 없습니다!");
    else
      loop = false;
  }

  // 연락처 파일의 EOF로 이동한다.
  // 실패하였을 경우, 에러를 출력한 후 종료한다.
  if (fseek(f, 0, SEEK_END) != 0) {
    perror("Failed to seek the file");
    exit(EXIT_FAILURE);
  }

  // 이름과 번호를 연락처 파일의 최하단에 저장한다.
  fprintf(f, "%s,%s\n", name, phone);
  printf("%s님을 연락처에 성공적으로 저장하였습니다.\n", name);
}