/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-04-28     jonas        the first version
 */

#include "sensor_maxim_max6675.h"
#include "max6675.h"

#define DBG_TAG "sensor.maxim.max6675"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

#define SENSOR_TEMP_RANGE_MAX (1024)
#define SENSOR_TEMP_RANGE_MIN (0)


#ifndef RT_SENSOR_VENDOR_MAXIM
#define RT_SENSOR_VENDOR_MAXIM (8u)
#endif

static rt_size_t _max6675_polling_get_data(rt_sensor_t sensor, struct rt_sensor_data *data)
{
    float temperature;
    struct max6675_device *max6675_dev = sensor->parent.user_data;

    if (sensor->info.type == RT_SENSOR_CLASS_TEMP)
    {
        temperature = max6675_read_temp(max6675_dev);
        data->data.temp = (rt_int32_t)(temperature * 100);
        data->timestamp = rt_sensor_get_ts();
    }else 
        return 0;
    return 1;
}

static rt_size_t max6675_fetch_data(struct rt_sensor_device *sensor, void *buf, rt_size_t len)
{
    RT_ASSERT(buf);

    if (sensor->config.mode == RT_SENSOR_MODE_POLLING)
    {
        return _max6675_polling_get_data(sensor, buf);
    }
    else
        return 0;
}

static rt_err_t max6675_control(struct rt_sensor_device *sensor, int cmd, void *args)
{
    rt_err_t result = RT_EOK;

    return result;
}

static struct rt_sensor_ops sensor_ops =
{
    max6675_fetch_data,
    max6675_control
};

int rt_hw_max6675_init(const char *name, struct rt_sensor_config *cfg)
{
    rt_int8_t result;
    rt_sensor_t sensor_temp = RT_NULL; 
    struct max6675_device *max6675_dev;

#ifdef PKG_USING_MAX6675

    max6675_dev = max6675_init(cfg->intf.dev_name);
    if (max6675_dev == RT_NULL)
    {
        return -RT_ERROR;
    }
    
    /* temperature sensor register */
    sensor_temp = rt_calloc(1, sizeof(struct rt_sensor_device));
    if (sensor_temp == RT_NULL)
        return -RT_ERROR;
    
    sensor_temp->info.type       = RT_SENSOR_CLASS_TEMP;
    sensor_temp->info.vendor     = RT_SENSOR_VENDOR_UNKNOWN;
    sensor_temp->info.model      = "max6675";
    sensor_temp->info.unit       = RT_SENSOR_UNIT_DCELSIUS;
    sensor_temp->info.intf_type  = RT_SENSOR_INTF_SPI;
    sensor_temp->info.range_max  = SENSOR_TEMP_RANGE_MAX;
    sensor_temp->info.range_min  = SENSOR_TEMP_RANGE_MIN;
    sensor_temp->info.period_min = 5;

    rt_memcpy(&sensor_temp->config, cfg, sizeof(struct rt_sensor_config));
    sensor_temp->ops = &sensor_ops;

    result = rt_hw_sensor_register(sensor_temp, name, RT_DEVICE_FLAG_RDONLY, max6675_dev);
    if (result != RT_EOK)
    {
        LOG_E("device register err code: %d", result);
        goto __exit;
    }

#endif

    return RT_EOK;

__exit:
    if (sensor_temp)
        rt_free(sensor_temp);
    return -RT_ERROR;
}