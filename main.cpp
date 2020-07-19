#include <stdint.h> // for uint32_t
#include <stdio.h> // for printf
#include <netinet/in.h> // for ntohl

// 실행파일 사용법 출력
void printUsage(char *exename) {
	printf("Syntax : %s <file1> <file2>\n", exename);
	printf("Sample : %s a.bin b.bin\n", exename);
}

// uint8_t 배열을 host byte order의 uint32_t형 숫자로 반환
uint32_t bufToNum_hbo(uint8_t *buf) {
	uint32_t *p = reinterpret_cast<uint32_t*>(buf);
	uint32_t num = *p;
	return ntohl(num);
}

int main(int argc, char *argv[]) {
	if (argc != 3) {
		printUsage(argv[0]);
		return 0;
	}
	
	FILE *file1, *file2;

	if ((file1 = fopen(argv[1], "r")) == NULL) {
		printf("Can't open file %s\n", argv[1]);
		printUsage(argv[0]);
		return 0;
	}

	if ((file2 = fopen(argv[2], "r")) == NULL) {
		printf("Can't open file %s\n", argv[2]);
		printUsage(argv[0]);
		return 0;
	}

	uint8_t buffer[4] = { 0x00, 0x00, 0x00, 0x00 };
	int len;
	uint32_t num1, num2;

	if ((len = fread(buffer, sizeof(uint8_t), 4, file1)) != 4) {
		printf("%s file should be at least 4 bytes\n", argv[1]);
		return 0;
	}
	num1 = bufToNum_hbo(buffer);

	if ((len = fread(buffer, sizeof(uint8_t), 4, file2)) != 4) {
		printf("%s file should be at least 4 bytes\n", argv[2]);
		return 0;
	}
	num2 = bufToNum_hbo(buffer);
	
	fclose(file2);
	fclose(file1);

	uint32_t sum_val = num1 + num2;

	printf("%u(0x%x) + %u(0x%x) = %u(0x%x)\n", num1, num1, num2, num2, sum_val, sum_val);
	return 0;
}
