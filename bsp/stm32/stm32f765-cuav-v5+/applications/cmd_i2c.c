
#include <rtthread.h>
#include <rtdevice.h>
#include <string.h>
#include <stdio.h>

#define AHT10_I2C_BUS_NAME          "i2c1"
static struct rt_i2c_bus_device *i2c_bus = RT_NULL;     /* I2C?????? */
static rt_bool_t initialized = RT_FALSE;                /* ???????? */


static rt_err_t write_reg(struct rt_i2c_bus_device *bus, rt_uint16_t addr, rt_uint8_t reg, rt_uint8_t *data)
{
    rt_uint8_t buf[3] = {0x55, 0x99, 0x11};
    struct rt_i2c_msg msgs;
    buf[0] = reg; //cmd
    buf[1] = data[0];
    buf[2] = data[1];
    msgs.addr = addr;
    msgs.flags = RT_I2C_WR;
    msgs.buf = buf;
    msgs.len = 3;

    if (rt_i2c_transfer(bus, &msgs, 1) == 1)
    {
        return RT_EOK;
    }
    else
    {
        return -RT_ERROR;
    }
}

static void i2c_bus_init(const char *name)
{
    i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(name);
    if (i2c_bus == RT_NULL)
    {
        rt_kprintf("can't find %s device!\n", name);
    }
    else
    {
        initialized = RT_TRUE;
    }
}

static void i2c_write(int argc, char *argv[])
{
    char *bus_number = NULL;
    rt_err_t ret;

    if (argc != 2) {
        rt_kprintf("please input: i2c_write <bus_number> .\n");
        return;
    }

    bus_number = argv[1];

    if (strlen(bus_number) != 1 || bus_number[0] < '0' || bus_number[0] > '9') {
        rt_kprintf("please input currect bus num(0~9).\n");
        return;
    }


    char name[RT_NAME_MAX];
    sprintf(name, "i2c%s", bus_number);

    if (!initialized)
    {
        i2c_bus_init(name);
    }
    rt_kprintf("i2c bus name is %s.\n", name);
		
    if (initialized)
    {
        ret = write_reg(i2c_bus, 0x1a, 0xff, NULL);
        if (ret != RT_EOK) {
            rt_kprintf("write i2c addr failed!\n", name);
        }
    }
    else
    {
        rt_kprintf("initialize %s failed!\n", name);
    }
}

MSH_CMD_EXPORT(i2c_write, write i2c cmd);
