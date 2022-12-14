#include "focaltech_core.h"
#include "focaltech_flash.h"

/*ft_rw_iic_drv main device number*/
#define FTS_CHAR_DEVICE_NAME			"ft_rw_iic_drv"
#define FTS_CHAR_DEVICE_MAJOR			210
#define FTS_I2C_RDWR_MAX_QUEUE			36
#define FTS_I2C_SLAVEADDR			11
#define FTS_I2C_RW				12

/*create apk debug channel*/
#define PROC_UPGRADE				0
#define PROC_READ_REGISTER			1
#define PROC_WRITE_REGISTER			2
#define PROC_AUTOCLB				4
#define PROC_UPGRADE_INFO			5
#define PROC_WRITE_DATA				6
#define PROC_READ_DATA				7
#define PROC_SET_TEST_FLAG			8
#define FTS_DEBUG_DIR_NAME			"fts_debug"
#define PROC_NAME				"ftxxxx-debug"
#define WRITE_BUF_SIZE				512
#define READ_BUF_SIZE				512

static u8 proc_operate_mode = PROC_UPGRADE;
static struct proc_dir_entry *focal_proc_entry;

/************************************************************************
*	Name: focal_proc_write
*  Brief:interface of write proc
* Input: file point, data buf, data len, no use
* Output: no
* Return: data len
***********************************************************************/
static ssize_t focal_proc_write(struct file *filp,
const char __user *buff, size_t count, loff_t *ppos)
{
	u8 writebuf[WRITE_BUF_SIZE];
	int buflen = count;
	int writelen = 0;
	int ret = 0;
	char upgrade_file_path[512];
	struct ts_kit_device_data *focal_dev_data = NULL;
	struct focal_platform_data *focal_pdata = NULL;

	focal_dev_data = focal_get_device_data();
	focal_pdata = focal_get_platform_data();
	if(buflen >= WRITE_BUF_SIZE){
		TS_LOG_DEBUG("%s:buff size is too large",	__func__);
		return -EINVAL;
	}
	if (copy_from_user(&writebuf, buff, buflen)) {
		TS_LOG_DEBUG("%s:copy from user error",	__func__);
		return -EFAULT;
	}
	proc_operate_mode = writebuf[0];

	switch (proc_operate_mode) {
	case PROC_UPGRADE:
		memset(upgrade_file_path, 0, sizeof(upgrade_file_path));
		sprintf(upgrade_file_path, "%s", writebuf + 1);
		upgrade_file_path[buflen-1] = '\0';
		TS_LOG_DEBUG("%s",  upgrade_file_path);

		disable_irq(focal_dev_data->ts_platform_data->irq_id);
		ret = focal_flash_upgrade_with_bin_file(focal_pdata,
			upgrade_file_path);
		enable_irq(focal_dev_data->ts_platform_data->irq_id);
		if (ret < 0) {
			TS_LOG_DEBUG("%s:upgrade failed.",  __func__);
			return ret;
		}

	    break;
	case PROC_SET_TEST_FLAG:
		break;
	case PROC_READ_REGISTER:
		writelen = 1;
		ret = focal_write(writebuf + 1, writelen);
		if (ret < 0) {
			TS_LOG_DEBUG("%s:write iic error",  __func__);
			return ret;
		}
		break;
	case PROC_WRITE_REGISTER:
		writelen = 2;
		ret = focal_write(writebuf + 1, writelen);
		if (ret < 0) {
			TS_LOG_DEBUG("%s:write iic error",  __func__);
			return ret;
		}
		break;
	case PROC_AUTOCLB:
		TS_LOG_DEBUG("%s: autoclb",  __func__);
		break;
	case PROC_READ_DATA:
	case PROC_WRITE_DATA:
		writelen = count - 1;
		if (writelen > 0) {
			ret = focal_write(writebuf + 1, writelen);
			if (ret < 0) {
				TS_LOG_DEBUG("%s:write iic error",  __func__);
				return ret;
			}
		}
		break;
	default:
		break;
	}

	return count;
}

/* interface of read proc */
/************************************************************************
*	Name: focal_proc_read
*  Brief:interface of read proc
* Input: point to the data, no use, no use, read len, no use, no use
* Output: page point to data
* Return: read char number
***********************************************************************/
static ssize_t
focal_proc_read(struct file *filp, char __user *buff, size_t count, loff_t *ppos)
{
	int ret = 0;
	size_t num_read_chars = 0;
	int readlen = 0;
	u8 regvalue = 0x00, regaddr = 0x00;
	u8 buf[READ_BUF_SIZE] ={0};

	switch (proc_operate_mode) {
	case PROC_UPGRADE:
		regaddr = 0xA6;
		ret = focal_read_reg(regaddr, &regvalue);
		if (ret < 0)
			num_read_chars =
			sprintf(buf, "%s", "get fw ver failed.");
		else
			num_read_chars =
			sprintf(buf, "cur fw ver:0x%02x", regvalue);
		break;
	case PROC_READ_REGISTER:
		readlen = 1;
		ret = focal_read(NULL, 0, buf, readlen);
		if (ret < 0) {
			TS_LOG_DEBUG("%s:read iic error",  __func__);
			return ret;
		}
		num_read_chars = 1;
		break;
	case PROC_READ_DATA:
		readlen = count;
		ret = focal_read(NULL, 0, buf, readlen);
		if (ret < 0) {
			TS_LOG_DEBUG("%s:read iic error",  __func__);
			return ret;
		}

		num_read_chars = readlen;
		break;
	case PROC_WRITE_DATA:
		break;
	default:
		break;
	}
	TS_LOG_DEBUG("%s:num_read_chars =%d ,count = %d",  __func__ ,num_read_chars,count);
	if (num_read_chars > count){
		TS_LOG_ERR("%s:buff size is too large",  __func__);
		return -EINVAL ;
	}
	if (copy_to_user(buff, buf, num_read_chars)) {
		TS_LOG_DEBUG("%s:copy to user error",  __func__);
		return -EFAULT;
	}

	return num_read_chars;
}
static const struct file_operations focal_proc_fops = {
		.owner	= THIS_MODULE,
		.read	= focal_proc_read,
		.write	= focal_proc_write,
};

static int focal_proc_init(void);
static void focal_proc_exit(void);

/*******************************************************************************
* functions body
*******************************************************************************/

int focal_apk_node_init(void)
{
	int err = 0;

	/* init proc */
	err = focal_proc_init();
	if (err < 0)
		return err;

	return 0;
}

int focal_apk_node_exit(void)
{
	focal_proc_exit();
	return 0;
}

/************************************************************************
* Name: focal_create_apk_debug_channel
* Brief:  create apk debug channel
* Input: i2c info
* Output: no
* Return: success =0
***********************************************************************/
static int focal_proc_init(void)
{

	focal_proc_entry = proc_create(PROC_NAME, 0664, NULL, &focal_proc_fops);

	if (NULL == focal_proc_entry) {
		TS_LOG_INFO("Couldn't create proc entry!");
		return -ENOMEM;
	} else {
		TS_LOG_INFO("Create proc entry success!");
	}

	return 0;
}
/************************************************************************
* Name: focal_release_apk_debug_channel
* Brief:  release apk debug channel
* Input: no
* Output: no
* Return: no
***********************************************************************/
static void focal_proc_exit(void)
{
	if (focal_proc_entry)
		proc_remove(focal_proc_entry);
}

