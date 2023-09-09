#include "i8253.h"
#include "i8259A.h"
#include "io.h"
#include "myPrintk.h"
#include "tick.h"
#include "uart.h"
#include "vga.h"
#include "wallClock.h"

#define ERRCOLOR 0x04
#define EXIT_SIGNAL 0x00FF

typedef struct myCommand {
	char name[80];
	char help_content[200];
	int (*func)(int argc, char (*argv)[8]);
} myCommand;

int cmdfinder(const char* const cmd);  // define here,to find the order of cmd

int strcmp(const char* a, const char* b) {
	while ((*a != 0) && (*b != 0)) {
		if ((*a) != (*b)) return (int)(*a - *b);
		a++;
		b++;
	}
	return (int)(*a - *b);
}

int func_cmd(int argc, char (*argv)[8]);
myCommand cmd = {"cmd\0", "List all command\n\0", func_cmd};

int func_exit(int argc, char (*argv)[8]) {
	if (argc == 1) {
		return EXIT_SIGNAL;  // shutdown the shell
	} else {
		myPrintk(ERRCOLOR, "Error: exit: illegal number of args.\n\n");
		return -1;
	}
}
myCommand EXIT = {"exit\0", "shut off the shell\n\0", func_exit};

// one special command, should be listed at the 2nd last,function show at last
int func_help(int argc, char argv[8][8]);
myCommand help = {"help\0",
				  "Usage: help [command]\n\0Display info about [command]\n\0",
				  func_help};

// one special command, should be listed at the last
int func_not_found(int argc, char (*argv)[8]) {  // illegal command
	myPrintk(ERRCOLOR, "Error: your input \"%s\" is not a correct command.\n\n",
			 argv[0]);
}
myCommand not_found = {"not found\0", "Not a command.\0", func_not_found};

myCommand* cmdlist[] = {&cmd, &EXIT, &help, &not_found};  // command list
const int cmdnum = sizeof(cmdlist) / sizeof(myCommand*);  // command number
int cmdfinder(const char* const cmd) {
	for (int i = 0; i < cmdnum; i++) {
		if (strcmp(cmd, cmdlist[i]->name) == 0) return i;
	}
	return (int)(cmdnum - 1);  // not find,return no command
}

int func_help(int argc, char (*argv)[8]) {
	// finished
	if (argc == 1) {  // only one arg
		myPrintk(0x07, "%s\n", help.help_content);
		return 0;
	} else if (argc == 2) {  // two args to display
		int order = cmdfinder(argv[1]);
		if (order != (cmdnum - 1)) {
			myPrintk(0x07, "%s\n", cmdlist[order]->help_content);
			return 0;
		} else {  // not a command
			myPrintk(ERRCOLOR,
					 "Error: help: your input \"%s\" is not a correct "
					 "command.\n\n",
					 argv[1]);
			return -1;
		}
	} else {  // arg num error
		myPrintk(ERRCOLOR, "Error: help: illegal number of args.\n\n");
		return -1;
	}
}
int func_cmd(int argc, char (*argv)[8]) {
	// finished
	if (argc == 1) {
		myPrintk(0x07, "All commands listed:\n");
		// list commands
		for (int i = 0; i < cmdnum - 1; i++)
			myPrintk(0x03, "%s\n",
					 cmdlist[i]->name);  // display all commands in blue

		myPrintk(0x07, "\n");
		return 0;
	} else {  // arg error
		myPrintk(ERRCOLOR, "Error: cmd: illegal number of args.\n\n");
		return -1;
	}
}

int _isspace(char ch) {
	if (ch == ' ' || ch == '\t')
		return 1;
	else
		return 0;
}

int spilt(const char* const buffer, char arr[8][8]) {  // return argc
	if (buffer[0] == 0) return -1;                     // empty string

	int argc = 0, index = 0, count = 0;

	while (_isspace(buffer[count]) && count <= 255)
		count++;  // clean the first spaces of the buffer

	if (count == 256 || buffer[count] == 0) return -1;  // syntax error

	for (; count <= 255 && buffer[count] != 0 && argc <= 7; count++) {
		if (_isspace(buffer[count])) {  // deal with space
			arr[argc][index] = 0;       // end the string
			while (_isspace(buffer[count]))
				count++;  // goto next pos of
						  // no-space
			if (buffer[count] == 0)
				return ++argc;  // if end, return

			else {  // not end
				argc++;
				index = 0;
				if (argc == 8) return -1;  // if more args than allowed
			}
		}
		arr[argc][index] = buffer[count];
		index++;
		if (index == 8) return -1;  // if more chars than allowed
	}
	arr[argc][count + 1] = 0;
	return ++argc;
}

void startShell(void) {
	//我们通过串口来实现数据的输入
	// finished

	do {
		char BUF[256] = {0};  //输入缓存区
		int BUF_len = 0;      //输入缓存区的长度

		int argc = 0;
		char argv[8][8] = {0};
		BUF_len = 0;
		myPrintk(0x07, "Student>> \0");
		while ((BUF[BUF_len] = uart_get_char()) != '\r') {
			uart_put_char(BUF[BUF_len]);  //将串口输入的数存入BUF数组中
			append_char_to_screen(BUF[BUF_len], 0x07);
			BUF_len++;  // BUF数组的长度加
		}
		append_char_to_screen('\n', 0x07);
		BUF[BUF_len] = 0;  // end the string

		uart_put_chars(" -pseudo_terminal\0");
		uart_put_char('\n');

		// myPrintk(0x07,"%s\n",BUF);

		argc = spilt(BUF, argv);
		if (argc <= 0) {
			myPrintk(ERRCOLOR, "Error: command cannot be dealt.\n\n");
		} else {
			int order = cmdfinder(argv[0]);
			if (cmdlist[order]->func(argc, argv) == EXIT_SIGNAL) return;
		}

		// OK,助教已经帮助你们实现了“从串口中读取数据存储到BUF数组中”的任务，接下来你们要做
		//的就是对BUF数组中存储的数据进行处理(也即，从BUF数组中提取相应的argc和argv参
		//数)，再根据argc和argv，寻找相应的myCommand
		//***实例，进行***.func(argc,argv)函数 调用。

		//比如BUF中的内容为 “help cmd”
		//那么此时的argc为2 argv[0]为help argv[1]为cmd
		//接下来就是 help.func(argc, argv)进行函数调用即可

		//谢谢助教

	} while (1);
}
