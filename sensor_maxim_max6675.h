/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-04-28     jonas        the first version
 */

#ifndef SENSOR_MAXIM_MAX6675_H__
#define SENSOR_MAXIM_MAX6675_H__

#include <sensor.h>
#include "max6675.h"



int rt_hw_max6675_init(const char *name, struct rt_sensor_config *cfg);

#endif



