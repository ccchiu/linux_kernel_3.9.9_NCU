/* https://unix.stackexchange.com/questions/223888/how-to-add-a-system-call-in-linux-kernel-4-x */

#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#define __NR__csie_ncu_survey_TT 352
static inline long csie_ncu_survey_TT(void) { return syscall(__NR__csie_ncu_survey_TT,NULL); }

int main()
{
	int i,j, array[100][100];                  

	for(i=0;i<100;i++)
		for(j=0;j<100;j++)
			array[i][j]=i*j;	
		
	csie_ncu_survey_TT();
	return 0;
}
