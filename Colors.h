/* 
 *  
 *   Color Codes for Printing 
 *   Status Messages To The Terminal
 *
 */

#ifndef __COLORS_H__
#define __COLORS_H__

#define COLOR_SIZE sizeof(COLORS[1]) / sizeof(char)
static const int num_colors = 6;

typedef enum color_index{RESET,RED,GREEN,YELLOW,BLUE,PURPLE,CYAN} Color_Index;
static const char* COLORS[] = {"\033[0m","\033[1;31m","\033[1;32m","\033[1;33m","\033[1;34m","\033[1;35m","\033[1;36m"};

#endif
