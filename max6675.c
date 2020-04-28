/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-04-28     jonas        the first version
 */

#include <rtthread.h>
#include <rthw.h>
#include <rtdevice.h>
#include <finsh.h>

#include <sensor.h>
#include "max6675.h"

#ifdef PKG_USING_MAX6675

#define DBG_ENABLE
#define DBG_SECTION_NAME "max6675"
#define DBG_LEVEL DBG_LOG
#define DBG_COLOR
#include <rtdbg.h>

#ifndef RT_USING_SPI
#error "Please eable RT_USING_SPI"
#endif


static rt_err_t read_data(struct rt_spi_device *dev, rt_uint8_t *buf)
{
    rt_spi_recv(dev, buf, 2);
    return RT_EOK;
}

float max6675_read_temp(max6675_device_t dev)
{
    rt_uint8_t temp[2];
    float temperature = 0.0;
    rt_err_t result;
    
    result = rt_mutex_take(dev->lock, RT_WAITING_FOREVER);
    if(result == RT_EOK)
    {
        read_data(dev->spi, temp);
        if(!((temp[0] & 0x80) || (temp[1] &0x02)))
        {
            if(!(temp[1] & 0x04))
            {
                temperature = ((temp[0] << 8 | temp[1]) >> 3) / 4.0;
            }
        }
    }else
    {
        LOG_E("Failed to reading tempurature");
    }
    rt_mutex_release(dev->lock);
    
    return temperature;
}

/**
 * This function initializes max6675 registered device driver
 *
 * @param dev the name of max6675 device
 *
 * @return the max6675 device.
 */
max6675_device_t max6675_init(const char *spi_device_name)
{
    max6675_device_t dev;

    RT_ASSERT(spi_device_name);

    dev = rt_calloc(1, sizeof(struct max6675_device));
    if (dev == RT_NULL)
    {
        LOG_E("Can't allocate memory for max6675 device on '%s' ", spi_device_name);
        rt_free(dev);

        return RT_NULL;
    }
    dev->spi = SPI_DEVICE(rt_device_find(spi_device_name));

    if (dev->spi == RT_NULL)
    {
        LOG_E("Can't find max6675 device on '%s'", spi_device_name);
        rt_free(dev);
        return RT_NULL;
    }

    dev->lock = rt_mutex_create("mutex_max6675", RT_IPC_FLAG_FIFO);
    if (dev->lock == RT_NULL)
    {
        LOG_E("Can't create mutex for max6675 device on '%s'", spi_device_name);
        rt_free(dev);
        return RT_NULL;
    }

    return dev;
}

/**
 * This function releases memory and deletes mutex lock
 *
 * @param dev the pointer of device driver structure
 */
void max6675_deinit(max6675_device_t dev)
{
    RT_ASSERT(dev);

    rt_mutex_delete(dev->lock);

    rt_free(dev);
}


void max6675(int argc, char *argv[])
{
    static max6675_device_t dev = RT_NULL;

    if (argc > 1)
    {
        if (!strcmp(argv[1], "probe"))
        {
            if (argc > 2)
            {
                /* initialize the sensor when first probe */
                if (!dev || strcmp(dev->spi->parent.parent.name, argv[2]))
                {
                    /* deinit the old device */
                    if (dev)
                    {
                        max6675_deinit(dev);
                    }
                    dev = max6675_init(argv[2]);
                }
            }
            else
            {
                rt_kprintf("max6675 probe <dev_name>   - probe sensor by given name\n");
            }
        }
        else if (!strcmp(argv[1], "read"))
        {
            if (dev)
            {
                float temp_data;

                /* read the sensor */
                temp_data = max6675_read_temp(dev);
                if (temp_data == 0)
                {
                    rt_kprintf("object is not proximity of sensor \n");
                }
                else
                {
                    rt_kprintf("max6675 read : %d.%d\n", (uint16_t)temp_data, ((uint16_t)(temp_data * 100)) % 100);
                }

            }
            else
            {
                rt_kprintf("Please using 'max6675 probe <dev_name>' first\n");
            }
        }
        else
        {
            rt_kprintf("Unknown command. Please enter 'max6675' for help\n");
        }
    }
    else
    {
        rt_kprintf("Usage:\n");
        rt_kprintf("max6675 probe <dev_name>   - probe sensor by given name\n");
        rt_kprintf("max6675 read               - read sensor max6675 data\n");
    }
}
MSH_CMD_EXPORT(max6675, max6675 sensor function);

#endif /* PKG_USING_MAX6675 */

