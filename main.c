#include "DP.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

static void usage(){
	fprintf(stderr,
		"\nUsage: bg  [options]\n\n"
		"Options:\n"
		"\t-b barcode size\n"
		"\t-m mismatches allowed\n"
		"\t-N number of barcodes need to be balanced\n"
		"\t-B number of bases need to be balanced\n"
		"\t-S [optional] strict number, 7 : everything, 3: no trimer,1: no dimer , 0 : no neartri\n"
		"e.g.\n./bg -b 8 -m 3 -N24 -B 5 \n"
		);
	exit(-1);
}

int main(int64_t argc, char *argv[]){
	int bz=0, mm=0, strict=0, nb=0, ib=0;
	
 	int c;
	while((c=getopt(argc, argv, "b:m:N:B:S:"))!=-1){
		switch(c){

			case 'b':
				bz=atoi(optarg);	
				break;
			case 'm':
				mm=atoi(optarg);	
				break;
			case 'N':
				nb=atoi(optarg);	
				break;
			case 'B':
				ib=atoi(optarg);	
				break;
			case 'S':
				strict=atoi(optarg);	
				break;
			case '?':
				fprintf(stderr, "Unknown -%c option  %s\n",optopt, optarg);
				usage();
				break;
			default:
				usage();
				break;
		}		
	}
	fprintf(stderr, "b %d m %d N %d B %d S %d\n", bz, mm, nb,ib, strict);
	if(  ( bz==0 || mm==0 || nb==0 ||ib==0)) usage();	
	srand(time(NULL));
	solution_t *ss=solve(  bz, mm, strict);	
	int bval;
	int runs=100;
	int mins[runs];
	{
		int i, min_i;
		int ** vecs=(int **) malloc(sizeof(int*)*runs);
		for(i=0;i<runs;++i){	
			int *bl=balance( nb, ib, &bval, ss);
			vecs[i]=bl;
			mins[i]=bval;
		}
		int min=10000;
		for(i=0;i<runs;++i){
			if (mins[i]<min){
				min=mins[i];	
				min_i=i;
			}
		}	
		fprintf(stderr, "Min Balanced Value %d\n",min);
		int cnt[ib][4];
		int *vec=vecs[min_i];	
		{
			int i, j, ibn;
			for (j=0;j<ib*4 ;j++){cnt[j/4][j%4]=0; }	
			for(i=0;i<ss->m;++i){
				if (vec[i]!=1) continue ;
					int n=ss->s[i] 	;
				// kick AGCT count into cnt
				j=0;
				ibn=0;
				do{
					int t=n%4;
					cnt[ibn++][t]++;
					n/=4;
					j++;
				}while(j<ss->bz && ibn<ib);
			}
		}	
		{///check balance
			int i, j;
			for(i=0;i<ib;++i){
				for(j=0;j<4;j++){
					printf( "%d " ,cnt[i][j]  )  ;
					}
					printf("\n");
				}
		}
		{///print best balanced
			int i;
			char *sbuff=(char *)malloc(sizeof(char)*(bz+1));
			fprintf(stderr,"##############%d Balanced##############\n",nb);
			for(i=0;i<ss->m;++i){
				if (vecs[min_i][i]==1)
					printf("%s\n",_ntoc( ss->s[i], bz, sbuff));
			}
			fprintf(stderr,"##############%d remains##############\n",ss->m-nb) ;
			
			for(i=0;i<ss->m;++i){
				if (vecs[min_i][i]!=1)
					printf("%s\n",_ntoc( ss->s[i], bz, sbuff));
			}
			free( sbuff);
			
		}	
			
		for(i=0;i<runs;++i){	
			free(vecs[i]);
		}	
		free(vecs);
	}
	solution_destroy(ss);
	return 1;
}
