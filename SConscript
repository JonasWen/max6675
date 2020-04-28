from building import *
Import('rtconfig')

cwd     = GetCurrentDir()
src	= Glob('*.c')
path = [cwd]

group = DefineGroup('max6675', src, depend = ['PKG_USING_MAX6675'], CPPPATH = path)

Return('group')