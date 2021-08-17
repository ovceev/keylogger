
static int keylogger_init(void) {
    return 0;
}

static void keylogger_exit(void) {
    return;
}

module_init(keylogger_init);
module_exit(keylogger_exit);