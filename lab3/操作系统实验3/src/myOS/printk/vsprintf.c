#include <stdarg.h>

int mySprintf(char* dst, const char* format, va_list args) {
	int stack[20];
	int count;
	char* q = dst;
	for (const char* p = format; *p; ++p) {
		if (*p == '%' && *(p + 1) == 'd') {
			count++;
			p++;
			int num = va_arg(args, int);
			if (num < 0) {
				*q = '-';
				q++;
				num = -num;
			}
			int n = 0;
			while (num >= 10) {
				stack[n] = num % 10;
				num /= 10;
				n++;
			}
			stack[n] = num;
			for (int i = n; i >= 0; --i) {
				*q = '0' + stack[i];
				q++;
			}
		} 
		else if(p[0]=='%'&&p[1]=='s'){
			count++;
			p++;
			char* str=va_arg(args,char*);
			while(*str!=0){
				*q=*str;
				q++;
				str++;
			}
		}
		else {
			*q = *p;
			++q;
		}
	}
	*q = '\0';
	return count;
}