#include <locale.h>
#include "ContactAdd.h"
#include "ContactDelete.h"
#include "ContactModify.h"
#include "ContactSearch.h"

enum SWITCH {
  LOOP_SEARCH = 1,
  LOOP_INSERT,
  LOOP_UPDATE,
  LOOP_DELETE,
  LOOP_QUIT
};

void MainLoop() {
  const char *filename = "2012007729_우한샘.csv";
  char buf[1024];
  Contact *c = (Contact *)malloc(sizeof(Contact));
  if (!Contact_Init(c, filename)) {
    puts("구조체 생성 오류!");
    return;
  }

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
        c->ContactSearch(c);
        break;
      case LOOP_INSERT:
        c->ContactInsert(c);
        break;
      case LOOP_UPDATE:
        c->ContactUpdate(c);
        break;
      case LOOP_DELETE:
        c->ContactDelete(c);
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

  MainLoop();
  return 0;
}
