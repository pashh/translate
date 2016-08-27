/*
 translate -- the translation of the text in the console by means of dictionaries of stardict. 
 
 Copyright (C) 2016 Naidolinsky Dmitry <naidv88@gmail.com>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 3, or (at your option)
 any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <string.h>
#include <ftw.h>
#include <ctype.h>
#include <stdlib.h>
#include <termios.h>
#include <asm-generic/ioctls.h>
#include <sys/ioctl.h>

#define DIR 1
#define LIST 2

struct configs {
	        char *dirs;
					        char *dicts;
};

const static char copying[]="<translate> Copyright (C) <2016> <Naidolinsy Dmitry>\n"
    "This program comes with ABSOLUTELY NO WARRANTY.\n"
		"This is free software, and you are welcome to redistribute it\n"
		"under certain conditions;\n";

#define COM 1
#define KREF 2

void parser(char *,int);
char *parserc;
char *word;

int nfile = 0;
int print_file(const char *file)
{
	struct winsize ws;
	ioctl(0,TIOCGWINSZ, &ws);
	char *ptr;
	char *line = calloc(ws.ws_col + 14,sizeof(char));
	memset(line,45,ws.ws_col  + 14);
	ptr = line;
	ptr += ws.ws_col / 2;
	char *cfile = calloc(strlen(file) + 18, sizeof(char));
	ptr = ptr - strlen(file) + (strlen(file) / 2);
	sprintf(cfile,"\033[1;33m%s\033[1;36m",file);
	strncpy(ptr,cfile,strlen(cfile));
	printf("\033[1;36m%s\033[0m\n",line);

}
int process(const char *file)
{

	FILE *dictionary;
	if ((dictionary = fopen(file,"r")) == NULL){
		perror("fopen");
		return 0;
	}


	char *target = calloc(128 ,sizeof(char));
	sprintf(target, "<k>%s</k>",word);
	char line[1024];
	char *ptr;
	char *tran;
	char *check;
	
	int no = 0;
	int found=0;
	int more = 0;
	while (fgets(line, 1024,dictionary) != NULL){
		if (found == 0)
		if (ptr = strstr(line, target)){
			print_file(file);
			check = ptr;
			found = 1;
			if (( ptr = strstr(ptr, "<dtrn>"))==NULL){
				if (ptr < check)
				continue;
			}
			else goto in;

		}
		if (found == 1){
			tran = ptr;
			if (ptr = strstr(line,"<k>")){
				if (ptr = strstr(line,"<dtrn>")){
					no++;
					more = 1;
					goto in;
				}
				if (ptr = strstr(line,"<ex>")){
					no++;
					more = 1;
					goto primes;
				}
				if (ptr = strstr(line,"<kref>")){
					more = 1;
					goto kref;
				}
				*ptr = '\0';
end:
				if (more)
					more = 0;
				parser(tran,0);
				
				fclose(dictionary);
				return 0;
			}
				if (ptr = strstr(line,"<dtrn>")){
					no++;
in:
					tran = ptr;
					if (more)
						goto end;
					parser(tran,0);
					continue;
				}

			if (ptr = strstr(line,"<ex>")){
				no++;
primes:
				tran = ptr;
				if(more)
					goto end;
				parser(tran,COM);
				continue;
			}
			

				if (ptr = strstr(line,"<kref>")){
kref:
					tran = ptr;
					if (more)
						goto end;
					parser(tran,KREF);
					continue;
				}
		}
	}
			fclose(dictionary);
			return 0;
}

struct b {
	unsigned int i:1;
};
void parser(char *current, int com)
{
	int inc = 0;
	int check = 0;
	int colors[6];
	int color = 0;
	if(com == COM)
		printf("\033[0;36m");
	else
		if(com == KREF)
			printf("\033[1;32m");
		else
	printf("\033[1;37m");
	struct b b;
	b.i = 0;
	while(*current != '\0'){
		if(strncmp(current,"<k>",3)==0){
			printf("\033[0;37m");
			printf("\n");
			return ;
		}

		if(strncmp(current,"<ex>",4)==0){
			inc = 1;
			colors[color]=COM;
			color++;
			current += 3;
			printf("\033[0;36m");
			goto skip;
		}

		if(strncmp(current,"</ex>",5)==0){
			inc = 0;
			color--;
			current += 4;
			printf("\033[0;37m");
			goto skip;
		}

		if(strncmp(current,"<kref>",6)==0){
			inc = 1;
			colors[color]=KREF;
			color++;
			current += 5;
			printf("\033[1;32m");
			goto skip;
		}
		
		if(strncmp(current,"</kref>",7)==0){
			inc = 0;
			color--;
			current += 6;
			printf("\033[0;37m");
			goto skip;
		}

		if(strncmp(current,"<co>",4)==0){
			inc = 1;
			colors[color]=3;
			color++;
			current += 3;
			printf("\033[5;33m");
			goto skip;
		}

		if(strncmp(current,"</co>",5)==0){
			inc = 0;
			color--;
			current += 4;
			printf("\033[0;37m");
			goto skip;
		}

		if(strncmp(current,"<abr>",5)==0){
			inc = 1;
			colors[color]=4;
			color++;
			current += 4;
			printf("\033[0;33m");
			goto skip;
		}

		if(strncmp(current,"</abr>",6)==0){
			inc = 0;
			color--;
			current += 5;
			printf("\033[0;37m");
			goto skip;
		}

		if ( (*current == '<') || (*current == '>') ){
			if (*current == '<' ) parserc = current;
			b.i = ~b.i;
			current++;
			continue;
		}
		if ( b.i == 0 ){
			if(strncmp(current, "&quot;",6)==0){
				current += 5;
				printf("\"");
				goto skip;
			}
			if(*current != '\n'){

				if(check!=color)
				switch(color){
					case 0:	if(inc==0)printf("\033[0;37m");break;
					case 1: if(inc==0)printf("\033[0;36m");break;
					case 2: if(inc==0)printf("\033[1;32m");break;
					case 3: if(inc==0)printf("\033[5;33m");break;
					case 4: if(inc==0)printf("\033[0;33m");break;
				}
				check = color;
			printf("%c",*current);
			}
		}
skip:
		current++;
	}
	printf("\033[0;37m");
	printf("\n");
}

struct configs * getconfig();
struct configs *cf;

int found(const char *fpath, const struct stat *sb,
		int typeflag, struct FTW *ftwbuf){
	char *start;
	char *end;
	int symbol = 0;
	if (cf->dicts != NULL){
		end = cf->dicts;
		start = end;
		while(*end != '\0'){
			end++;
			if (symbol == 1)
				if (!isspace(*end))
					start = end,symbol=0;
			if ((*end == ',') || (*end == '\0')){
				char *name = calloc(end - start, sizeof(char));
				strncpy(name,start,end - start);

				if (strncmp(&fpath[ftwbuf->base],name,end - start)==0)
					printf("%s\n",fpath);
				free(name);
				start = end+1;
				symbol = 1;
			}
		}
	}else
		if (sb->st_mode & S_IFREG)
		//printf("%s\n",fpath);
		process(fpath);
	
	return 0;
}

int main(int argc, char *argv[])
{
	if (argc < 2){
		printf("\033[1m%s\033[0m",copying);
		return 0;
	}
	word = argv[1];
	cf = getconfig();

	char *punct = cf->dirs;
	char *start = punct;
	char *currentdir;
	int symbol = 0;
	while(*punct != '\0'){
		punct++;
		if (symbol == 1)
			if (!isspace(*punct))
				start = punct,symbol=0;
		if ((*punct == ',') || (*punct == '\0')){
			currentdir = calloc(punct - start, sizeof(char));
			strncpy(currentdir,start,punct - start);
			nftw(currentdir, found,20, 0); 

			if (*punct == '\0')
				return 0;
			punct += 1;
			
			start = punct;
			if (isspace(*start))
			symbol = 1;
		}

	}
	return 0;
}

struct configs * getconfig()
{
	char *cpath = calloc(strlen(getenv("HOME")) + strlen(".translate.conf"),sizeof(char));
	sprintf(cpath,"%s/.translate.conf",getenv("HOME"));

	struct configs *cf = calloc(1,sizeof(struct configs));
	FILE *conf;
	if ((conf = fopen(cpath, "r"))==NULL){
		perror("fopen");
		return 0;
	}
	char line[255];
	char *ptr;
	int options=0;

	while(fgets(line,254,conf)!=NULL){
		if ((ptr = strstr(line,"jobdir")) && (line[0] != '#')){
			if (isalpha(*(ptr + 6))){
				int ind;
				for (ind = 0; isalpha(*(ptr + ind));ind++);
				*(ptr + ind)='\0';
				fprintf(stderr,"\033[5;31mнеизвестная опция %s\n"
											 "возможно вы имели ввиду jobdir\033[0m\n",ptr);
				continue;
			}
			options += DIR;
			if((ptr = strchr(line,'='))!=NULL)
			if (*(ptr + 1) == '=')
				fprintf(stderr,"\033[5;31mне может быть == в опции\033[0m\n");
			ptr++;
			for(;!isprint(*ptr);ptr++);
			char *end = ptr;
			for(;*end != '\n';end++);
			cf->dirs = calloc(end - ptr + 2, sizeof(char));
			strncpy(cf->dirs,ptr,end - ptr);
			continue;
		}
		if ((ptr = strstr(line,"dicts")) && (line[0] != '#')){
			if (isalpha(*(ptr + 5))){
				int ind;
				for (ind = 0; isalpha(*(ptr + ind));ind++);
				*(ptr + ind)='\0';
				fprintf(stderr,"\033[5;31mнеизвестная опция %s\n"
											 "возможно вы имели ввиду dicts\033[0m\n",ptr);
				continue;
			}
			options += LIST;
			if((ptr = strchr(line,'='))!=NULL)
			if (*(ptr + 1) == '=')
				fprintf(stderr,"\033[5;31mне может быть == в опции\033[0m\n");
			ptr++;
			for(;!isprint(*ptr);ptr++);
			char *end = ptr;
			for(;*end != '\n';end++);
			cf->dicts = calloc(end - ptr + 2, sizeof(char));
			strncpy(cf->dicts,ptr,end - ptr);
			continue;
		}

	}
	fclose(conf);

	if (options == 0){
		/* нет настроек */
	}else
		if ( (options & DIR) && (options & LIST) ){
			/* две опции */
		}else
			if (options & DIR){
				/* все файлы в рабочем каталоге */
			}else
				if (options & LIST){
					/* все перечисленные файлы */
				}
	return cf;
}
