#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/unistd.h>
#include <linux/sched.h>

#define TAG "week10"
#define TIMER_TIMEOUT 1

static struct timer_list mytimer;
unsigned long seconds = 1;

static void timer_handler(struct timer_list *t1) {
   static size_t nseconds;
   nseconds += TIMER_TIMEOUT;
   pr_info("[timer_hanler] nseconds = %d\n", nseconds);

   mod_timer(t1, jiffies + TIMER_TIMEOUT * HZ);
}

int init_module(void) {
   pr_info("[timer_init] Init module\n");
   timer_setup(&mytimer, timer_handler, 0);
   mod_timer(&mytimer, jiffies + TIMER_TIMEOUT * HZ);
   return 0;
}

void cleanup_module(void) {
   printk(KERN_INFO "Cleanup %s\n", TAG);
   del_timer(&mytimer);
}

// module_init(init_module);
// module_exit(cleanup_module);
