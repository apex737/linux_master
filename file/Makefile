CC = gcc
CFLAGS = -W -Wall

# 1. 현재 디렉토리의 모든 .c 파일 목록 (file1.c file2.c ...)
SRCS = $(wildcard *.c)

# 2. .c 확장자를 제거한 실행 파일 이름 목록 (file1 file2 ...)
# patsubst(패턴, 바꿀문자, 대상) 함수를 사용합니다.
PROGS = $(patsubst %.c, %, $(SRCS))

# 3. 'make'만 치면 PROGS 리스트에 있는 모든 파일을 다 만들겠다는 뜻
all: $(PROGS)

# 4. 개별 컴파일 규칙 (패턴 규칙)
# %는 와일드카드와 같습니다. file1은 file1.c로부터 만든다는 의미입니다.
%: %.c
	$(CC) $(CFLAGS) -o $@ $<

# 5. 청소 규칙 (이전 요청하신 find 명령 적용)
clean:
	find . -maxdepth 1 -type f ! -name "*.c" ! -name "Makefile" -delete

.PHONY: all clean