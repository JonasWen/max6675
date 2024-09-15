from building import *
Import('rtconfig')

cwd     = GetCurrentDir()
src	= []
src += Glob('max6675.c')
if GetDepend('PKG_MAX6675_USING_SENSOR_V1'):
    src += ['maxim_max6675_sensor_v1.c']
path = [cwd]

group = DefineGroup('max6675', src, depend = ['PKG_USING_MAX6675'], CPPPATH = path)

Return('group')