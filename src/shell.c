#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>

#include "shell.h"
#include "color.h"
#include "launch.h"
#include "commands/ls.h"
#include "commands/cd.h"
#include "config.h"

// 输入缓存，当前输入的内容
char input_buffer[INPUT_BUFFER_SIZE];
// 当前命令，第一个输入的内容
char current_command[INPUT_BUFFER_SIZE];
// 当前工作路径
char current_working_directory[MAX_PATH_LENGTH];
// 当前参数
char *current_arguments[MAX_ARGUMENT_NUMBER];

int return_value = 0;
bool exited = false;
int position = 0;

/**
* 处理当前输入缓存里的输入，将输入信息写入 current_command 和 current_arguments中
*/
void process_input()
{
	int argument_index = 0;
	int current_position = 0;
	current_arguments[0] = malloc(MAX_PATH_LENGTH);
	for (int i = 0; i <strlen(input_buffer); i++)
	{
		if (input_buffer[i] == ' ')
		{
			// 终止当前命令
			current_arguments[argument_index][current_position] = 0;
			argument_index++;
			current_position = 0;
			current_arguments[argument_index] = malloc(MAX_PATH_LENGTH);
			continue;
		}
		current_arguments[argument_index][current_position] = input_buffer[i];
		current_position++;
	}

	// 终止当前命令
	current_arguments[argument_index][current_position] = 0;
	current_arguments[argument_index +1] = 0;
	strcpy(current_command, current_arguments[0]);
}

void free_input(){
}

/**
* 检查所给命令是否和输入命令相同
* @param command 需要查询的命令
*/
bool check_command(char *command)
{
	return strcmp(current_command, command) == 0;
}

/**
* 检查输入的命令是否在 PATH 中有对应的程序
*/
bool exist_in_path()
{
	return get_executable_path(current_command) != NULL;
}

/**
* 检查指定字符串是否输入命令的前辍
*/
bool start_with(char *prefix)
{
	return strncmp(prefix, current_command, strlen(prefix)) == 0;
}

/**
* 获取当前文件名称
*/
void get_current_folder(char *output)
{
	int last_slash = 0;
	int cwd_length = strlen(current_working_directory);
	for (int i=0; i<cwd_length; i++)
	{
		if (current_working_directory[i] == '/')
		{
			last_slash = i;
		}
	}
	for (int i = last_slash +1; i<cwd_length; i++)
	{
		output[i - last_slash -1] = current_working_directory[i];
	}
	output[cwd_length - last_slash -1] = 0;
}

/**
* 准备一行新的输入
*/
void restart_input()
{
	position = 0;
	getcwd(current_working_directory, sizeof(current_working_directory));
	char current_folder[MAX_PATH_LENGTH];
	get_current_folder(current_folder);
	printf(ANSI_COLOR_BLUE "%s " ANSI_COLOR_YELLOW	 "> " ANSI_COLOR_RESET, current_folder);
}

/** 
* 处理按键
* @param c 按键代码
*/
bool process_key(unsigned char c)
{
	if (c == '\n') // Newline 回车
	{
		printf("\n");
		return true;
	}
	else if (c==127 || c==8) // Delete键或者 Backspace键
	{
		if (position > 0)
		{
			printf("\b \b");
			position--;
		}
	}
	else if(c == '\t') // Tab 键
	{
	}
	else if(c == 27) // Escape 键
	{
		char next = getchar();
		if (next == 91)
		{
			switch (getchar())
			{
				case 65:
					break;
			}
		}
		else
		{
			return process_key(next);
		}
	}
	else // 其它按键
	{
		printf("%c", c);
		// 加进输入缓存里
		input_buffer[position] = c;
		position++;
	}
	return process_key(getchar());
}

/**
* Shell程序主循环
*/
void line_loop()
{
	restart_input();
	while (true)
	{
		if (process_key(getchar()))
		{
			break;
		}
	}

	input_buffer[position] = 0;
	process_input();
	
	if (check_command("exit"))
	{
		exited = true;
		return_value = 0;
	}
	else if (check_command("ls"))
	{
		command_ls();
	}
	else if (check_command("cd"))
	{
		command_cd(current_arguments[1]);
	}
	else if (check_command("pwd"))
	{
		printf("%s\n", current_working_directory);
	}
	else if(check_command("clear"))
	{
		printf("\e[1;1H\e[2]");
	}
	else if(exist_in_path() || start_with("./"))
	{
		if (!start_with("./")){
			// 替换完整路径
			current_arguments[0] = get_executable_path(current_command);
		}
		launch_process(current_arguments);
	}
	else
	{
		if (strlen(current_command))
		{
			printf("Command \"%s\" is not found.\n", current_command);
		}
	}
	free_input();
}

/**
* 输入循环
*/
int input_loop()
{
	while (!exited)
	{
		line_loop();
	}
	printf("Bye. \n\n");
	return return_value;
}

/**
* 响应 SIGINT 信号
*/
void sigint_handler()
{
	if (is_child_process_running()){
	}
	else
	{
		printf("\nPlease use \"exit\" command to exit the shell.\n");
		printf("\n");
		restart_input();
	}
}

		
