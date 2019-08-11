#include<signal.h>
#include<stdio.h>

#include "shell.h"
#include "tio.h"

// 程序入口
int main()
{
	// 禁用输出缓存，因为默认 Linux的程序输出会有缓存，这样输出的内容可能不会显示在屏幕上
	// 因为 Shell 是呼唤程序，所以这里要禁用程序输出的缓存
	setbuf(stdout, NULL);
	setbuf(stderr, NULL);
	
	// 这里还需要修改 Terminal IO的设置
	backup_tio();
	set_tio();

	// 绑定快捷键 Ctrl+C 发送的 SIGINT事件
	signal(SIGINT, sigint_handler);
	
	// 启动 Shell 的输入循环
	int return_value = input_loop();

	// 恢复之前的 TIO 设置
	restore_tio();

	// 结束程序
	return return_value;
}
