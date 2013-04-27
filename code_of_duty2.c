#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>

int getnumber(char* buf, int size, int delim, FILE* fp) {//suppose number have enough place in buffer
	int t=-1;
	int i=0;
	while( i<size ) {
		t=fgetc(fp);
		if(t==EOF) {
			if(feof(fp)) {
				/*printf("(%d) ", __LINE__);
				printf("End-Of-File.\n");//*/
			}
			else {
				fprintf(stderr, "(%d) ", __LINE__);
				perror("Can not read file.");
			}
			break;
		}
		if(t=='\r'||t=='\n'||t==delim) break;
		buf[i]=t;
		++i;
	}
	buf[i]='\0';
	return i;
}

void show(FILE* fp, const char const* tableau, int size) {
	if(size==0) return;
	fprintf(fp, "(");
	for(int i=0;i<size-1;++i) {
		fprintf(fp, "%d, ", tableau[i]);
	}
	fprintf(fp, "%d", tableau[size-1]);
	fprintf(fp, ")\n");
}

int main() {
	FILE* fp=fopen("input.txt", "rb");
	if(!fp) {
		fprintf(stderr, "[ERROR] Can not open file 'input.txt' for reading (%d).\n", __LINE__);
		return 1;
	}
	FILE* fp2=fopen("output2.txt", "wb");
	if(!fp2) {
		fprintf(stderr, "[ERROR] Can not open file 'output.txt' for writing (%d).\n", __LINE__);
		return 2;
	}
	char buf[5];//2 digits, \r\n, \0
	int res=0;
	int size=1;
	int number=0;
	while(size>0) {
		res=getnumber(buf, 5, ' ', fp);
		assert(buf[0]>='0');
		assert(buf[0]<='9');
		int size=(buf[0]-'0');
		if(buf[1]>='0'&&buf[1]<='9') {
			size*=10;
			size+=(buf[1]-'0');
		}
		memset(buf, '\0', 5);
		printf("[INFO] size=%d\n", size);
		if(size==0) {
			fprintf(stderr, "[INFO] end of file (%d).\n", __LINE__);
			break;
		}
		char* tableau=(char*)calloc(size, sizeof(char));
		int index=0;
		for(int i=0;i<size;++i) {
			res=getnumber(buf, 5, ' ', fp);
			number=(buf[0]-'0');
			if(res==2) {
				number*=10;
				number+=(buf[1]-'0');
			}
			memset(buf, '\0', 5);
			assert( res==1 || res==2 );
			tableau[index]=number;
			++index;
		}
		//tableau read
		int sum=0;
		for(int i=0;i<size;++i) {
			sum+=tableau[i];
		}
		printf("[INFO] sum=%d\n", sum);
		if(sum/size*size==sum) {
			printf("[INFO] équilibrage possible.\n");
			int avg=sum/size;
			printf("[INFO] valeur moyenne: %d\n", avg);
			res=0;
			for(int i=0;i<size;++i) {
				if(tableau[i]!=avg) res=1;
			}
			/*équilibrage start*/
			if(res==0) {
				printf("tableau déjà équilibré.\n");
			}
			int iter=0;
			fprintf(fp2, "%d : ", iter);
			show(fp2, tableau, size);
			while(res!=0) {
				res=0;
				for(int i=0;i<size;++i) {
					if(tableau[i]!=avg) res=1;
				}
				if(res==0) break;
				/*équilibrage start*/
				for(int i=0;i<size-1;++i) {
					if( tableau[i]>avg && tableau[i+1]<=avg ) {
							--tableau[i];
							++tableau[i+1];
					}
					/*else if( tableau[i]<avg && tableau[i+1]>=avg ) {
							++tableau[i];
							--tableau[i+1];
					}//*/
					else if( tableau[i]>avg && tableau[i+1]>=avg && tableau[i]>(tableau[i+1]+1) ) {
							--tableau[i];
							++tableau[i+1];
					}
					/*else if( tableau[i]<avg && tableau[i+1]<=avg && tableau[i]<(tableau[i+1]-1) ) {
							++tableau[i];
							--tableau[i+1];
					}//*/
					else if( i>0) {
						if( tableau[i]>avg && tableau[i-1]<=avg ) {
								--tableau[i];
								++tableau[i-1];
						}
						/*else if( tableau[i]<avg && tableau[i-1]>=avg ) {
								++tableau[i];
								--tableau[i-1];
						}//*/
						else if( tableau[i]>avg && tableau[i-1]>=avg && tableau[i]>(tableau[i-1]+1) ) {
								--tableau[i];
								++tableau[i-1];
						}
						/*else if( tableau[i]<avg && tableau[i-1]<=avg && tableau[i]<(tableau[i-1]-1) ) {
								++tableau[i];
								--tableau[i-1];
						}//*/
					}
				}
				{
					int i=size-1;
					//repeat start
					if( tableau[i]>avg && tableau[i-1]<=avg ) {
							--tableau[i];
							++tableau[i-1];
					}
					/*else if( tableau[i]<avg && tableau[i-1]>=avg ) {
							++tableau[i];
							--tableau[i-1];
					}//*/
					else if( tableau[i]>avg && tableau[i-1]>=avg && tableau[i]>(tableau[i-1]+1) ) {
							--tableau[i];
							++tableau[i-1];
					}
					/*else if( tableau[i]<avg && tableau[i-1]<=avg && tableau[i]<(tableau[i-1]-1) ) {
							++tableau[i];
							--tableau[i-1];
					}//*/
				}
				//repeat stop
				++iter;
				fprintf(fp2, "%d : ", iter);
				show(fp2, tableau, size);
				if(iter==15000) {
					printf("[INFO] probably error\n");
					break;
				}
			}
			/*équilibrage stop*/
		}
		else {
			printf("[INFO] équilibrage PAS possible.\n");
			fprintf(fp2, "-1\n");
		}
		res=getnumber(buf, 5, ' ', fp);
		assert(res==0);//empty line
		assert(buf[0]=='\0'||buf[0]=='\r'||buf[0]=='\n');
	}
	fclose(fp2);
	fp2=fopen("output2.txt", "rb");
	FILE* fp3=fopen("output.txt", "wb");
	long pos=0;//début iter
	long pos2=0;//fin iter
	long pos3=0;//temp
	char* buffer=(char*)calloc(6400, sizeof(char));
	number=-1;
	int number2=-2;
	int number3=-2;
	number=0;
	pos=ftell(fp2);
	while(!feof(fp2)) {
		memset(buffer, '\0', 6400);
		if(fgets(buffer, 6400, fp2)==NULL) {//lire ligne
			if(feof(fp2)) {
				/*printf("(%d) ", __LINE__);
				printf("End-Of-File.\n");//*/
			}
		}
		sscanf(buffer, "%d ", &number);//récupérer numéro iter
		if(number==-2) break;
		if(number>number2) {
			number2=number;
			pos3=ftell(fp2);//position dernière iter (après)
		}
		else {//plus petit = new tableau
			pos2=pos3;//set fin iter
			fprintf(fp3, "%d\n", number2);//dernière iter
			fseek(fp2, pos, SEEK_SET);//go début iter
			while(ftell(fp2)<pos2) {
				if(fgets(buffer, 6400, fp2)==NULL) {//lire ligne
					if(feof(fp2)) {
						/*printf("(%d) ", __LINE__);
						printf("End-Of-File.\n");//*/
					}
					break;
				}
				fprintf(fp3, "%s", buffer);
			}
			fprintf(fp3, "\n");
			pos=pos2;
			number=-1;
			number2=-2;
			number3=-2;
		}
		if(strcmp(buffer, "-1")==0||strcmp(buffer, "-1\r")==0||strcmp(buffer, "-1\n")==0||strcmp(buffer, "-1\r\n")==0||strcmp(buffer, "-1\n\r")==0) {
			fprintf(fp3, "-1\n\n");
			res=getnumber(buf, 5, ' ', fp);
			assert(res==0);
			if(feof(fp2)) break;
			res=getnumber(buf, 5, ' ', fp);
			assert(res==0);
			if(feof(fp2)) break;
			number=-1;
			number2=-2;
			number3=-2;
			pos=ftell(fp2);
			pos2=pos;
			pos3=pos;
			continue;
		}
	}
	free(tableau);
	fclose(fp3);
	fclose(fp2);
	fclose(fp);
	return 0;
}
//
