#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/export.h>
#include <linux/compiler.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/aio.h>
#include <linux/capability.h>
#include <linux/kernel_stat.h>
#include <linux/gfp.h>
#include <linux/mm.h>
#include <linux/swap.h>
#include <linux/mman.h>
#include <linux/pagemap.h>
#include <linux/file.h>
#include <linux/uio.h>
#include <linux/hash.h>
#include <linux/writeback.h>
#include <linux/backing-dev.h>
#include <linux/pagevec.h>
#include <linux/blkdev.h>
#include <linux/security.h>
#include <linux/cpuset.h>
#include <linux/hardirq.h> /* for BUG_ON(!in_atomic()) only */
#include <linux/memcontrol.h>
#include <linux/cleancache.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#include <asm/desc.h>
#include <asm/pgtable.h>
#include <asm/desc.h>


static int get_reg_info()
{

        unsigned long cr3,cr4;
        cr3 = read_cr3();
        cr4=read_cr4();
        printk(KERN_INFO "cr3=%08X \n",cr3);
        printk(KERN_INFO "cr4=%08X  ", cr4 );
        printk(KERN_INFO "PAE=%X  ", (cr4>>5)&1);
        printk(KERN_INFO "PSE=%X  ", (cr4>>4)&1);
        return	1;
}

static void get_pg_macro(void)
{
        printk("PAGE_OFFSET = 0x%lx\n", PAGE_OFFSET);
        printk("PGDIR_SHIFT = %d\n", PGDIR_SHIFT);
        printk("PUD_SHIFT = %d\n", PUD_SHIFT);
        printk("PMD_SHIFT = %d\n", PMD_SHIFT);
        printk("PAGE_SHIFT = %d\n", PAGE_SHIFT);
        printk("PTRS_PER_PGD = %d\n", PTRS_PER_PGD);
        printk("PTRS_PER_PUD = %d\n", PTRS_PER_PUD);
        printk("PTRS_PER_PMD = %d\n", PTRS_PER_PMD);
        printk("PTRS_PER_PTE = %d\n", PTRS_PER_PTE);
        printk("PAGE_MASK = 0x%lx\n", PAGE_MASK);
}

static void dump_pgd(unsigned short start_entry, unsigned short end_entry)
{

/*
 *             Linear_address
 *  +------+-------+-------+---------------+
 *  | PGD  |  PMD  |  PTE  |     Offset    |
 *  +------+-------+-------+---------------+
 *                         <---PAGE_SHIFT-->
 *                 <------PMD_SHIFT-------->
 *          <------PGDIR_SHIFT------------->
 *
 */


	unsigned long addr;
        unsigned short i, j;
        pgd_t *base = get_current()->mm->pgd;
        pgd_t *pgd;  //pgd
        pud_t *pud;  //pud
        pmd_t *pmd;  //pmd
        pte_t *pte;  //pte

        /* to find an entry in a page-table-directory */
        //#define pgd_index(addr)     ((addr) >> PGDIR_SHIFT)  //get entry in PGD
        //#define pgd_offset(mm, addr)    ((mm)->pgd + pgd_index(addr)) //get address in PGD

        /* 1111 1111 1111 1111 1111  1111 1111 1111 */
        for (i = start_entry; i < end_entry; i++) {
                //covert entry to address
                addr = i << PGDIR_SHIFT;
#if 0
                printk(KERN_INFO "add=[%08lX] ", addr);
#endif
                //base address + entry[i]
                pgd = base + i;
		//got offset int pud
                pud = pud_offset(pgd, addr);
		//got offset int pmd
                pmd = pmd_offset(pud, addr);
		//if pmd is invaild.
                if (!pmd_present(*pmd)) {
                        printk(KERN_INFO "[%04u] NULL\n", i);
                        continue;
                }
                /* 4M */
                if (pmd_large(*pmd)) {
                        printk(KERN_INFO "[%04u] 4M 0x%08lX\n", i, pmd_val(*pmd));
                        //DUMP 4M Table Entry
#if 0
                        for (j = 0; j < 2; j++) {
                                pte = pte_offset_kernel(pmd, addr | (j << PAGE_SHIFT));
                                if (!pte_present(*pte))
                                        continue;
                                printk(KERN_INFO "\t\t(4M)[%04u] 0x%08lX\n", j, pte_val(*pte));
                        }
#endif
                        continue;
                }

                printk(KERN_INFO "[%04u] Page Table 0x%08lX\n", i, pmd_val(*pmd));
                //DUMP PAGE Table Entry
                //for (j = 0; j < PTRS_PER_PTE; j++) {
                for (j = 0; j < 2; j++) {
                        //#define pte_offset_kernel(pmd,addr) (pmd_page_vaddr(*(pmd)) + pte_index(addr))
                                       pte = pte_offset_kernel(pmd, addr | (j << PAGE_SHIFT));
                        //pte = pte_offset_kernel(pmd, addr);
                        if (!pte_present(*pte))
                                continue;
                        printk(KERN_INFO "\t\t[%04u] 0x%08lX\n", j, pte_val(*pte));
                }
        }
}
asmlinkage int sys_csie_ncu_survey_TT(void)
{
        printk(KERN_INFO "By jerry@NCU %s %s %s\n",__func__ , __DATE__,__TIME__);
        get_reg_info();
        get_pg_macro();
        dump_pgd(768,1024);
        return 1;
}
