#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/unistd.h>
#include <linux/sched.h>
#include <linux/ktime.h>
#include <linux/timekeeping.h>
#include <linux/gfp.h>
#include <linux/mm_types.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>

// Part 2 task: type getconf PAGE_SIZE
#define PAGE_SIZE 4096
// Then I created my own struct containing array of
// 8 unsigned integers and found it's size using sizeof() fucntion.
struct mystruct {
   unsigned int arrayOfIntegers[8];
};

// Part 1 task: I used module_param(name, type, perm)
// to pass parameters to the module when installed.
// Name - name of parameter, type - type of parameter,
// Perm is permision which is specififed in octal form.
// For example 0660 - allows user and groups to read/write.
static int param = 2000; //default value = 2000
module_param(param, int, 0660);

// Part 3 task:
// Same as part 3&4, but memory allocation is done by slab allocation.
// The first step is to create cache for the struct mystruct using
// kmem_cache_create, which returns pointer to the chache of type kmem_cache
// if successful and NULL if not.
struct kmem_cache *km;
void kmallocAlloc(int size) {
   struct timespec t1, t2;
   // First argument is name. Second is size of objects to be created in this cache.
   // Third argument is align - required alignment for the objects.
   // SLAB_HWCACHE_ALIGN | SLAB_PANIC | SLAB_POISON | SLAB_RED_ZONE are flags:
   // SLAB_HWCACHE_ALIGN - align the objects in this cache to a hardware cache line,
   // SLAB_PANIC - causes the slab layer to panic if the allocation fails. Usefull when
   // the allocation must not fail.
   // SLAB_POISON - causes the slab layer to fill the slab with a known value (a5a5a5a5).
   // This is called poisoning and is uuseful for caching access to uninitialized memory.
   // SLAB_RED_ZONE - redzones following object. Is used to detect writes after the object.
   // When module is got unloaded kmem_cache_destroy function removes the cache completely.
   km = kmem_cache_create("slabAllocation", size, 0, 
                        SLAB_HWCACHE_ALIGN|SLAB_PANIC|SLAB_POISON|SLAB_RED_ZONE, 
                        NULL);
   getnstimeofday(&t1);
   int i;
   char *buf;
   for(i = 0; i < param; i++) {
       buf = kmem_cache_alloc(km, GFP_KERNEL);
       kmem_cache_free(km, buf);
   }
   getnstimeofday(&t2);
   printk(KERN_INFO "Time when we started: %lu. Time when we finished: %lu. How much did it take: %lu.", t1.tv_nsec, t2.tv_nsec, t2.tv_nsec - t1.tv_nsec);
}

int init_module(void) {
   printk(KERN_INFO "Starting Lab 10 - Slab Allocation.\n");
   printk(KERN_INFO "Starting part 1:");
   printk(KERN_INFO "My module param: .%d\n", param);
   struct mystruct m;
   int temp1 = PAGE_SIZE/sizeof(m);
   printk(KERN_INFO "Starting part 2:");
   printk(KERN_INFO "The page size is: %d. Size of my struct: %d. Number if instances of my struct would fit in with in a single kernel memory page: %d.\n", PAGE_SIZE, sizeof(m), temp1);
   printk(KERN_INFO "Starting part 3:");
   kmallocAlloc(sizeof(m));
   return 0;
}

void cleanup_module(void) {
   printk(KERN_INFO "Cleanup module. End of lab 10.\n");
   kmem_cache_destroy(km);
}