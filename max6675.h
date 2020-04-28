/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-04-28     jonas        the first version
 */
#ifndef __DRV_MAX6675_H__
#define __DRV_MAX6675_H__

#include <rthw.h>
#include <rtthread.h>

struct max6675_device
{
    struct rt_spi_device *spi;

#ifdef AP3216C_USING_HW_INT

    ap3216c_int_cb als_int_cb;
    ap3216c_int_cb ps_int_cb;

#endif /* AP3216C_USING_HW_INT */

    rt_mutex_t lock;
};
typedef struct max6675_device *max6675_device_t;

max6675_device_t max6675_init(const char *spi_device_name);
float max6675_read_temp(max6675_device_t dev);

#endif



