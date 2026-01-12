CC = gcc
CFLAGS = -W -Wall

# 디렉토리 내의 모든 C 파일
SRCS = $(wildcard *.c)

# 최종 실행 파일
TARGET = a.out

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

# .c 파일과 Makefile을 제외한 모든 파일 삭제 (이전 요청 반영)
clean:
	find . -maxdepth 1 -type f ! -name "*.c" ! -name "Makefile" -delete

.PHONY: all clean
