#include "ipc/completion.h"
#include "drivers/can.h"

#define SAMPLE_CAN_NAME       "can1"      /* 串口设备名称 */

static int can_sample(int argc, char *argv[])
{

    rt_err_t ret = RT_EOK;
    char can_name[RT_NAME_MAX];
		struct can_configure cfg = {0};
		static rt_device_t can_dev;            /* CAN 设备句柄 */
		struct rt_can_msg msg = {0};           /* CAN 消息 */

		
    if (argc == 2)
    {
        rt_strncpy(can_name, argv[1], RT_NAME_MAX);
    }
    else
    {
        rt_strncpy(can_name, SAMPLE_CAN_NAME, RT_NAME_MAX);
    }

		can_dev = rt_device_find(can_name);
		if (!can_dev)
		{
        rt_kprintf("find %s failed!\n", can_name);
        return RT_ERROR;
		}
		/* 以中断接收及发送模式打开 CAN 设备 */
		ret = rt_device_open(can_dev, RT_DEVICE_FLAG_INT_TX | RT_DEVICE_FLAG_INT_RX);
	  if (RT_EOK != ret)
		{
        rt_kprintf("open %s failed!\n", can_name);
        return RT_ERROR;
		}
		
		cfg.mode = RT_CAN_MODE_LOOPBACK;
		cfg.baud_rate = CAN100kBaud;
		
		ret = rt_device_control(can_dev, RT_DEVICE_CTRL_CONFIG, &cfg);
	  if (RT_EOK != ret)
		{
        rt_kprintf("set cfg %s failed!\n", can_name);
        return RT_ERROR;
		}
		
		msg.id = 0x78;              /* ID 为 0x78 */
    msg.ide = RT_CAN_STDID;     /* 标准格式 */
    msg.rtr = RT_CAN_DTR;       /* 数据帧 */
    msg.len = 8;                /* 数据长度为 8 */
    /* 待发送的 8 字节数据 */
    msg.data[0] = 0x00;
    msg.data[1] = 0x11;
    msg.data[2] = 0x22;
    msg.data[3] = 0x33;
    msg.data[4] = 0x44;
    msg.data[5] = 0x55;
    msg.data[6] = 0x66;
    msg.data[7] = 0x77;
    /* 发送一帧 CAN 数据 */
    rt_device_write(can_dev, 0, &msg, sizeof(msg));
	
		rt_device_close(can_dev);
		
    return ret;
}

/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(can_sample, can device sample);
