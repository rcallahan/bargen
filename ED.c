#include "ED.h"
uint64_t FD(char *s1, int64_t sz1 ,char *s2){
	
	uint64_t i, score=0;
	for (i=0;i<sz1; i++){
		if (s1[i]!=s2[i] ) score++;
	}
	return score;
}
int  ED(char *s1, char *s2){
	int64_t sz1=strlen(s1);
	int64_t sz2=strlen(s2);
	if (sz1<sz2) return ED(s2, s1);
	if (!sz1) return sz2;
	if (!sz2) return sz1;
	
	 int  i, j;
		
	static  int  matrix[33][33];
	
	for(i=0;i<=sz2;i++){
		matrix[0][i]=i;
	}
	for(i=0;i<=sz1;i++){
		matrix[i][0]=i;
	}
		
	for(i=1; i<=sz1; i++){
		for (j=1; j<=sz2;j++){
			matrix[i][j]=MIN3(matrix[i-1][j]+1 , matrix[i][j-1]+1, matrix[i-1][j-1]+ ((s1[i-1]==s2[j-1])?0:1 ) );
		}
	}
	return matrix[sz1][sz2];
}
