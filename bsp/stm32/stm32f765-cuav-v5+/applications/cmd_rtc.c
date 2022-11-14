/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       lihl
 * 2018-11-30     misonyo      first implementation.
 */
/*
 * 程序清单：这是一个 RTC 设备使用例程
 * 例程导出了 rtc_sample 命令到控制终端
 * 命令调用格式：rtc_sample
 * 程序功能：设置RTC设备的日期和时间，延时一段时间后获取当前时间并打印显示。
*/
#include <rtthread.h>
#include <rtdevice.h>
static int rtc_sample(int argc, char *argv[])
{
    static rt_device_t device = RT_NULL;
    rt_err_t rst = -RT_ERROR;
		struct timeval tv;


    /* default is 0 */
    tv.tv_sec = 0;
    tv.tv_usec = 0;

    /* optimization: find rtc device only first */
    if (device == RT_NULL)
    {
        device = rt_device_find("rtc");
    }

    /* read timestamp from RTC device */
    if (device != RT_NULL)
    {
            rst = rt_device_control(device, RT_DEVICE_CTRL_RTC_GET_TIME, &tv.tv_sec);
            rt_device_control(device, RT_DEVICE_CTRL_RTC_GET_TIMEVAL, &tv);
            rt_device_close(device);
    }
		
		rt_kprintf("RTC Time sec is: %#x\n", tv.tv_sec);
		rt_kprintf("RTC Time usec is: %#x\n", tv.tv_usec);

    return rst;
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(rtc_sample, rtc sample);
