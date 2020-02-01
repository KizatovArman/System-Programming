#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
// #include <linux/smp.h>
#include <linux/unistd.h>


/*
Challedge: find total number of cpu cores
*/
void print_total_cpu(void) {
unsigned int eax=11,ebx=0,ecx=1,edx=0;

asm volatile("cpuid"
        : "=a" (eax),
          "=b" (ebx),
          "=c" (ecx),
          "=d" (edx)
        : "0" (eax), "2" (ecx)
        : );

printk("Cores: %d\nThreads: %d\nActual thread: %d\n",eax,ebx,edx);
}




int init_module(void)
{
	printk("Lab 1: start.");

	printk("Begin");
	print_total_cpu();
	printk("End");

	return 0;
}

void cleanup_module(void)
{
	printk("Lab 1: end.");
}
