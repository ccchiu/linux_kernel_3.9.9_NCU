
#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <string.h>
#define __NR__csie_ncu_survey_TT 352
static inline long csie_ncu_survey_TT(void) { return syscall(__NR__csie_ncu_survey_TT,NULL); }



#define GREETING "How are you?\n"

main()
{ char  greeting_array[1000]; 

	strcpy(greeting_array, GREETING); 
	csie_ncu_survey_TT(); 
}  
