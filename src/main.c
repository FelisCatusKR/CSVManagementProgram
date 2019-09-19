#include <locale.h>
#include "ContactAdd.h"
#include "ContactSearch.h"

enum SWITCH { LOOP_SEARCH = 1, LOOP_ADD, LOOP_EDIT, LOOP_DELETE, LOOP_QUIT };

void MainLoop(FILE *f) {
  char buf[1024];
  bool loop = true;

  while (loop) {
    puts("======= 연락처 프로그램 =======");
    puts("1. 연락처 검색");
    puts("2. 연락처 추가");
    puts("3. 연락처 수정");
    puts("4. 연락처 삭제");
    puts("5. 프로그램 종료");
    puts("");
    printf("기능을 선택하세요: ");
    fgets(buf, sizeof buf, stdin);
    int sw;
    sscanf(buf, "%d", &sw);

    switch (sw) {
      case LOOP_SEARCH:
        ContactSearch(f);
        break;
      case LOOP_ADD:
        ContactAdd(f);
        break;
      case LOOP_EDIT:
        break;
      case LOOP_DELETE:
        break;
      case LOOP_QUIT:
        loop = false;
        break;
      default:
        puts("잘못된 입력입니다!");
    }
  }
}

int main() {
  // 프로그램의 locale을 한국어/UTF-8 인코딩으로 지정
  setlocale(LC_CTYPE, "ko_KR.UTF.8");

  FILE *f = fopen("2012007729_Hansaem_Woo.csv", "r+");
  if (!f) {
    perror("Failed to open the file");
    return EXIT_FAILURE;
  }

  MainLoop(f);

  fclose(f);
  return 0;
}
