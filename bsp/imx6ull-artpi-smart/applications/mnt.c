/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 */

#include <rtthread.h>

#ifdef RT_USING_DFS

#include <dfs_fs.h>
#include <dfs_romfs.h>

int mnt_init(void)
{
    if (dfs_mount("sd0", "/", "elm", 0, NULL) != 0)
    {
        rt_kprintf("Dir / mount failed!\n");
        return -1;
    }

    rt_thread_mdelay(200);
    if (dfs_mount(RT_NULL, "/mnt", "rom", 0, &romfs_root) != 0)
    {
        rt_kprintf("Dir /mnt mount failed!\n");
        return -1;
    }

    rt_kprintf("file system initialization done!\n");
    return 0;
}
INIT_ENV_EXPORT(mnt_init);
#endif
