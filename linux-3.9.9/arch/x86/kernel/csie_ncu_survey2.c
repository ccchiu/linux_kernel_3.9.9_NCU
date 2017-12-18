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


/* Multipliers for offsets within the PTEs */
#define PTE_LEVEL_MULT (PAGE_SIZE)
#define PMD_LEVEL_MULT (PTRS_PER_PTE * PTE_LEVEL_MULT)
#define PUD_LEVEL_MULT (PTRS_PER_PMD * PMD_LEVEL_MULT)
#define PGD_LEVEL_MULT (PTRS_PER_PUD * PUD_LEVEL_MULT)




#define walk_pmd_level(a,p) walk_pte_level(__pmd(pud_val(a)),p)
#define pud_large(a) pmd_large(__pmd(pud_val(a)))
#define pud_none(a)  pmd_none(__pmd(pud_val(a)))

#define walk_pud_level(a,p) walk_pmd_level(__pud(pgd_val(a)),p)
#define pgd_large(a) pud_large(__pud(pgd_val(a)))
#define pgd_none(a)  pud_none(__pud(pgd_val(a)))


#define NCU_START_PAGE_ADDR (0)
#define NCU_END_PAGE_ADDR   (1023)
#define NCU_RANGE  (NCU_END_PAGE_ADDR-NCU_START_PAGE_ADDR)+1
/* Page Size FLAG in page entry */
#define PAGE_SIZE_FLAG (7)



static int get_reg_info()
{

        unsigned long pgd_paddr,cr4;
        pgd_paddr = read_cr3();
        cr4=read_cr4();
        printk(KERN_INFO "cr3=%08X \n",pgd_paddr);
        printk(KERN_INFO "cr4=%08X  ", cr4 );
        printk(KERN_INFO "PAE=%X  ", (cr4>>5)&1);
        printk(KERN_INFO "PSE=%X  ", (cr4>>4)&1);
        return	1;
}

static void get_pgtable_macro(void)
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


static void walk_pte_level(pmd_t addr,unsigned long P)
{
        int i;
        pte_t *start;
        unsigned long start_phy_addr;
        unsigned long current_phy_address;
        unsigned long current_address;
        start_phy_addr =  PTE_PFN_MASK & native_pmd_val(addr);
        start = (pte_t *) pmd_page_vaddr(addr);
        //for (i = 0; i < PTRS_PER_PTE; i++) {
        for (i = 0; i < 2; i++) {
                pgprot_t prot = pte_pgprot(*start);
                current_address = (P + i * PTE_LEVEL_MULT);
                current_phy_address =(start_phy_addr + i * PTE_LEVEL_MULT);
                printk("PTE [%d] current_phy_addr  =0x%0.8lx   current_address=0x%0.8lx\n",i,current_phy_address,current_address);
                start++;
        }
}





static void walk_pgd_level(int start_entry,int end_entry)
{
        int i;
        pgd_t *start;
        pgdval_t curr_pgd;
        unsigned long start_phy_addr;
        unsigned long current_phy_address;

        printk("CR3= 0x%0lx, va_CR3 = 0x%0lx\n",
               read_cr3(), (unsigned long)__va(read_cr3()));


        start =get_current()->mm->pgd;

        //start = swapper_pg_dir;

        start_phy_addr = __pa(start);
        printk("PGT localtion in memory: phs = 0x%0.8lx, virt  = 0x%0.8lx\n",
               (unsigned long)__pa(start), (unsigned long)start);

        //	memset(&st, 0, sizeof(st));

        //for (i = 0; i < PTRS_PER_PGD; i++) {
        for (i = start_entry; i < end_entry ; i++) {
                //	st.current_address = normalize_addr(i * PGD_LEVEL_MULT);
                current_phy_address = (start_phy_addr + i * PGD_LEVEL_MULT);
                printk("curr phy addr =0x%.8lx  ==> ",current_phy_address);
                if (!pgd_none(*start)) {
                        pgprotval_t prot = pgd_val(*start) & PTE_FLAGS_MASK;
                        curr_pgd=pgd_val(*start);
                        printk("ENTY=%d PS=%d ",i,(curr_pgd>>PAGE_SIZE_FLAG) & 0x00000001);

                        if (pgd_large(*start) || !pgd_present(*start)) {
                                //note_page(m, &st, __pgprot(prot), 1);
                                printk("note note_page\n");

                        } else {
                                printk("start at =0x%0.8lx,addr=%0.8lx\n",start,i * PGD_LEVEL_MULT);
                                walk_pud_level( *start,
                                                i * PGD_LEVEL_MULT);
                                //walk_pud_level(*start,i * PGD_LEVEL_MULT);
                                //walk_pte_level(__pmd(pud_val(*start)));

                        }
                } else {
                        //	note_page(m, &st, __pgprot(0), 1);
                        printk("pgd none\n");
                }
                start++;
        }

        /* Flush out the last page */
        //	st.current_address = normalize_addr(PTRS_PER_PGD*PGD_LEVEL_MULT);
        //	note_page(m, &st, __pgprot(0), 0);
}



static void dump_pgd(unsigned short start_entry, unsigned short end_entry)
{
        unsigned long addr;
        unsigned short i, j;
        pgd_t *base = get_current()->mm->pgd;
        pgd_t *pgd;
        pud_t *pud;
        pmd_t *pmd;
        pte_t *pte;

        /* to find an entry in a page-table-directory */
        //#define pgd_index(addr)     ((addr) >> PGDIR_SHIFT)  //获得在pgd表中的索引
        //#define pgd_offset(mm, addr)    ((mm)->pgd + pgd_index(addr)) //获得pmd表的起始地址

        /* 1111 1111 1111 1111 1111  1111 1111 1111 */
        for (i = start_entry; i < end_entry; i++) {
		//covert entry to address 
                addr = i << PGDIR_SHIFT;
                printk(KERN_INFO "add=[%08lX] ", addr);
		//base address + entry[i]
                pgd = base + i;
                pud =pud_offset(pgd, addr);
                pmd = pmd_offset(pud, addr);

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
			/*
	       	                   Linear_address
			+------+-------+-------+---------------+	   
			| PGD  |  PMD  |  PTE  |     Offset    |
			+------+-------+-------+---------------+
			                       <---PAGE_SHIFT-->
			               <------PMD_SHIFT-------->
			        <------PGDIR_SHIFT------------->
			 */ 
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
        get_pgtable_macro();
        dump_pgd(768,1024);

        //	walk_pgd_level(NCU_START_PAGE_ADDR,NCU_END_PAGE_ADDR);
        //	pgd_t *start =get_current()->mm->pgd;
#if 0
        pgdval_t pgd_entry[NCU_RANGE];
        pteval_t pte_entry=0;
        for(i=0; i< NCU_RANGE ; i++)
                pgd_entry[i]=0;
#endif


#if  0
        for(i =  NCU_START_PAGE_ADDR; i <= NCU_END_PAGE_ADDR; i++) {
                pgd_entry[i]=pgd_val(*(start+i));
                printk(KERN_INFO "PAGE Entry[%d]=0x%.8lx PS=%d 2nd pte_add=0x%.8lx ,PTE_VAL=%lu  \n",
                       i,
                       (unsigned long)pgd_entry[i],
                       (pgd_entry[i]>>PAGE_SIZE_FLAG) & 0x00000001,
                       pgd_entry[i]>>12,
                       pgd_entry[i]>>12);
                printk("0x%.81x at ENTRY[%d]\n", pgd_index()
                       //Lve2
                       //printk(KERN_INFO "PAGE Entry[%d]=%lu  page_entry_value=%d\n",i,(unsigned long)e,pgd_entry[i]);
        }
#endif
               //	copy_to_user(result,testarr,1024*sizeof(int));
               return 1;
}
