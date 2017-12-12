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
	csie_ncu_survey_TT();
        return 0;
}
