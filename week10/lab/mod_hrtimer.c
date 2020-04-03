#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/hrtimer.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/jiffies.h>

MODULE_LICENSE("GPL");

#define INTERVAL_BETWEEN_CALLBACKS (100 * 10000000LL) //1000ms (scaled in ns)
#define NR_ITERATIONS 20

int arr[10] = {0, 1, 1, 2, 3, 5, 8, 13, 21, 34};
int counter = 1;

static struct hrtimer timer;
static ktime_t kt;

static s64 starttime_ns;
 
static enum hrtimer_restart my_hrtimer_callback( struct hrtimer *timer )
{
	printk("I'm in hrtimer handler. Counter now is %d\r\n", counter);
    // hrtimer_forward(timer, timer->base->get_time(), kt);
    s64 now_ns = ktime_to_ns(ktime_get());
    // hrtimer_forward(timer, now, tick_period);
    kt = ktime_set(0, arr[counter]*INTERVAL_BETWEEN_CALLBACKS);
    hrtimer_forward(timer, timer->_softexpires, kt);
    if(counter <= 9) {
        counter++;
    }
    return HRTIMER_RESTART;
}
 
static int init_module_hrtimer( void )
{
	printk("mod_hrtimer: installing module...\n");
	kt = ktime_set(0, INTERVAL_BETWEEN_CALLBACKS);
    hrtimer_init(&timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
    hrtimer_start(&timer, kt, HRTIMER_MODE_REL);
    timer.function = my_hrtimer_callback;
    return 0;
}
 
static void cleanup_module_hrtimer( void )
{
	int ret;
	ret = hrtimer_cancel( &timer );
	if (ret)
	printk("mod_hrtimer: The timer was still in use...\n");
	printk("mod_hrtimer: HR Timer module uninstalling\n");
}
 
module_init(init_module_hrtimer);
module_exit(cleanup_module_hrtimer);

