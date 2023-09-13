#include <linux/atomic.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/ioctl.h>
#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/types.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "procon"
#define DEVICE_CLASS DEVICE_NAME "_class"
#define DEVICE_DRIVER DEVICE_NAME "_driver"

#define READ_USE_COUNT _IOR('r', '1', int32_t *)
#define INITIALIZE_BUFFER _IOW('w', '1', int32_t *)

#define BUF_SIZE PAGE_SIZE

static dev_t dev;
static struct class *cls;
static struct cdev cdev;

static atomic_t use_count = ATOMIC_INIT(0);
static int head, tail, read_count;
static int ring_buffer[BUF_SIZE];
static spinlock_t lock;

struct readout {
  int index;
  int value;
};

DECLARE_WAIT_QUEUE_HEAD(wait_full);
DECLARE_WAIT_QUEUE_HEAD(wait_empty);

int open_device(struct inode *inode, struct file *fp)
{
  pr_info("open");
  atomic_inc(&use_count);
  pr_info("use_count: %d\n", atomic_read(&use_count));
  return 0;
}

int close_device(struct inode *inode, struct file *fp)
{
  pr_info("close");
  atomic_dec(&use_count);
  pr_info("use_count: %d\n", atomic_read(&use_count));
  return 0;
}

ssize_t read_device(struct file *fp, char *buf, size_t len, loff_t *offset)
{
  struct readout rdout;

  while (head == tail) {
    wait_event_interruptible(wait_empty, head != tail);
  }

  spin_lock(&lock);

  rdout.index = read_count++;
  rdout.value = ring_buffer[tail];
  tail = (tail + 1) % BUF_SIZE;

  copy_to_user(buf, &rdout, len);

  spin_unlock(&lock);

  wake_up_interruptible(&wait_full);

  return len;
}

ssize_t write_device(struct file *fp, const char *buf, size_t len, loff_t *offset)
{
  int kern_buffer;

  wait_event_interruptible(wait_full, (head + 1) % BUF_SIZE != tail);
  copy_from_user(&kern_buffer, buf, len);

  ring_buffer[head] = kern_buffer;
  head = (head + 1) % BUF_SIZE;

  wake_up_interruptible(&wait_empty);

  return len;
}

long ioctl_device(struct file *fp, unsigned int cmd, unsigned long arg)
{
  int _use_count;

  switch (cmd) {
    case READ_USE_COUNT:
      _use_count = atomic_read(&use_count);
      pr_info("use_count: %d\n", _use_count);
      if (copy_to_user((int *)arg, &_use_count, sizeof(int)) < 0) {
        pr_err("fail to copy use_count to user");
        return -1;
      }
      break;
    case INITIALIZE_BUFFER:
      pr_info("initialize buffer");
      head = tail = read_count = 0;
      break;
    default:
      pr_info("unknown command");
      break;
  }

  return 0;
}

struct file_operations fops = {
  .owner = THIS_MODULE,
  .open = open_device,
  .read = read_device,
  .write = write_device,
  .unlocked_ioctl = ioctl_device,
  .release = close_device,
};

static int __init procon_module_init(void)
{
  int ret;

  pr_info("procon_module_init");

  ret = alloc_chrdev_region(&dev, 0, 1, DEVICE_NAME);
  if (ret < 0) {
    pr_err("fail to register char device region");
    goto chrdev_err;
  }

  cls = class_create(THIS_MODULE, DEVICE_CLASS);
  if (!cls) {
    pr_err("fail to create dev class");
    goto class_err;
  }

  ;
  if (device_create(cls, NULL, dev, NULL, DEVICE_DRIVER) == NULL) {
    pr_err("fail to create device");
    goto device_err;
  }

  cdev_init(&cdev, &fops);
  ret = cdev_add(&cdev, dev, 1);
  if (ret < 0) {
    pr_err("fail to add character device");
    goto cdev_err;
  }

  return 0;

cdev_err:
  device_destroy(cls, dev);

device_err:
  class_destroy(cls);

class_err:
  unregister_chrdev_region(dev, 1);

chrdev_err:
  return -1;
}

static void __exit procon_module_exit(void)
{
  cdev_del(&cdev);
  device_destroy(cls, dev);
  class_destroy(cls);
  unregister_chrdev_region(dev, 1);
  pr_info("procon_module_exit");
}

module_init(procon_module_init);
module_exit(procon_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("nk");
MODULE_DESCRIPTION("kernel producer/consumer problem module");
MODULE_VERSION("1.0.0");