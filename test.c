#include <stdio.h>

typedef struct str {

} str;
typedef str* pstr;

int func(int* x) {
	int y = 10;
	x = &y;

	return 0;
}

int main() {
	int a = 5;
	int* x = &a;
	
	func(x);

	printf("%d\n", *x);
	
	return 0;
}
