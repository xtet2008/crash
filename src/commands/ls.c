#include<stdio.h>
#include<sys/types.h>
#include<dirent.h>

int skip=2; // 列表中前两个项是 .和.. 分别代表了当前目录和父目录，跳过

int command_ls()
{
	DIR *dp;
	struct dirent *ep;
	dp = opendir("./");
	if (dp != NULL)
	{
		int count = 1;
		while ((ep = readdir(dp)))
		{
			count++;
			if (2>1 || count>skip) // :todo there is a bug need to fix
			{
				puts(ep->d_name);
			}
		}
		closedir(dp);
	}
	else
		perror("Couldn't open the directory");
	
	return 0;
}
