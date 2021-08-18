#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/io.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ilia Kovalev");
MODULE_DESCRIPTION("keylogger for a test task");



static int keylogger_init(void) {
    printk("Hello\n");
    return 0;
}

static void keylogger_exit(void) {
    printk("Bye\n");
    return;
}

module_init(keylogger_init);
module_exit(keylogger_exit);