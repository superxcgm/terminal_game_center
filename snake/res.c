#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "res.h"

struct XC_Res *load_res(const char *path)
{
	int get_line_cnt(const char *path);
	int nlines = get_line_cnt(path);
	FILE *fp = fopen(path, "r");
	char buf[BUFSIZ];
	int i;
	struct XC_Res *res_rtn = (struct XC_Res *)malloc(sizeof(struct XC_Res));
	if(!fp){
		perror("File opening failed");
		exit(1);
	}
	res_rtn->cnt = nlines;
	res_rtn->data = (char **)malloc(res_rtn->cnt * sizeof(char *) + 1);
	i = 0;
	while(fgets(buf, BUFSIZ - 1, fp)){
		int len = strlen(buf);
		if(buf[len - 1] == '\n') buf[len - 1] = '\0';/* remove newline character */
		res_rtn->data[i] = (char *)malloc(len + 1);
		strcpy(res_rtn->data[i++], buf);
	}
	fclose(fp);
	return res_rtn;
}
int get_line_cnt(const char *path)
{
	FILE *fp = fopen(path, "r");
	int nlines = 1;
	int c;
	if(!fp){
		perror("File opening failed");
		exit(1);
	}
	while((c = fgetc(fp)) != EOF)
		if(c == '\n')
			++nlines;
	if(ferror(fp))
		fprintf(stderr, "I/O error when readding [%s]\n", path);
	fclose(fp);
	return nlines;
}