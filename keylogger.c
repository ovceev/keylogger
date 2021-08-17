#include <linux/init.h>
#include <linux/module.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ilia Kovalev");
MODULE_DESCRIPTION("keylogger for the test task");



static int keylogger_init(void) {
    return 0;
}

static void keylogger_exit(void) {
    return;
}

module_init(keylogger_init);
module_exit(keylogger_exit);