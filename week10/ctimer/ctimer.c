#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/unistd.h>
#include <linux/sched.h>
#include <linux/ktime.h>
#include <linux/timekeeping.h>


#define TAG "week10"
#define TIMER_TIMEOUT 1
#define NANO 1000000000L

static struct timer_list mytimer;
unsigned long seconds = 1;
struct timespec tv;

void print_current_time(void) {
   getnstimeofday(&tv);
   long long milliseconds = tv.tv_sec*1000LL;
   long long Seconds = milliseconds/1000;
   long long hours = (tv.tv_sec/3600 + 6)%24;
   long long minutes = (tv.tv_sec/60)%60;
   long long seconds = tv.tv_sec%60; 
   // long long hours = (tv.tv_sec/3600 + 6)/24;
   // long long minutes = tv.tv_sec/60%60;
   // long long seconds = tv.tv_sec%60; 
   // pr_info("milliseconds: %lld\n", milliseconds);
   pr_info("Current time: %lld:%lld:%lld", hours, minutes, seconds);
}

static void timer_handler(struct timer_list *t1) {
   static size_t nseconds;
   nseconds += TIMER_TIMEOUT;
   pr_info("------------------------------------------");
   pr_info("[timer_hanler] nseconds = %d\n", nseconds);
   print_current_time();
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
