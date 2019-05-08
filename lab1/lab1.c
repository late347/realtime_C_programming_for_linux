#include <stdio.h>
#include <dirent.h>

int main(int argc, char  *argv[])
{
	/* code */
	DIR * dp;
	struct dirent * dirp;
	printf("Directory contents were as follows:\n");
	if ( (dp = opendir(".")) != NULL )
	{
		/* code */
		while( (dirp = readdir(dp)) != NULL )
		{
			if (strcmp(dirp->d_name, ".") == 0 ||
				strcmp(dirp->d_name, "..") == 0)
			{
				continue;
			}
			printf("%s\n", dirp->d_name);
		}
		closedir(dp);
	}
	else
	{
		fprintf(stderr, "Cannot open the current dir\n" );
	}


	return 0;
}
