#include<unistd.h>
#include<termios.h>
#include<signal.h>

struct termios backup_tio_settings;

void backup_tio()
{
	tcgetattr(STDIN_FILENO, &backup_tio_settings);
}

void set_tio(){
	struct termios new_tio;
	
	// 创建新的设置
	new_tio = backup_tio_settings;
	new_tio.c_lflag &= (~ICANON & ~ECHO);
	
	// 设置TIO
	tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
}

void restore_tio()
{
	tcsetattr(STDIN_FILENO, TCSANOW, &backup_tio_settings);
}

