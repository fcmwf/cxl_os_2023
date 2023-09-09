#ifndef __VGA_H__
#define __VGA_H__

#define VGA_COL 80
#define VGA_ROW (25 - 1) 

#define VGA_BASE 0xB8000

#define VGA_SCREEN_WIDTH 80   // vga 屏幕宽度（可容纳字符数）
#define VGA_SCREEN_HEIGHT 25  // vga 屏幕高度

#define CURSOR_LINE_REG 0xE      // 行号寄存器
#define CURSOR_COL_REG 0xF       // 列号寄存器
#define CURSOR_INDEX_PORT 0x3D4  // 光标行列索引端口号
#define CURSOR_DATA_PORT 0x3D5   // 光标数据端口号

void update_cursor(void);
void clear_screen(void);
void scroll_screen(void);
void append_char_to_screen(char c, int color);
void append2screen(char* str, int color);
void set_cursor_pos(unsigned short pos);

#endif
