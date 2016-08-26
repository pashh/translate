/*
 translate -- the translation of the text by means of dictionaries of stardict
 
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COM 1
#define KREF 2

void parser(char *,int);
char *parserc;

int main(int argc,const char *argv[])
{
	if (argc < 3  ||  argc > 3){
		printf("%s словарь [слово | фраза]\n",argv[0]);
		return 0;
	}

	FILE *dictionary;
	if ((dictionary = fopen(argv[1],"r")) == NULL){
		perror("fopen");
		return 0;
	}

	char *word = argv[2];

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
