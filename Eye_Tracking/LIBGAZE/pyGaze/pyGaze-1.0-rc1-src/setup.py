import sys, os
from distutils.core import setup
from distutils.extension import Extension
from Pyrex.Distutils import build_ext

print "Platform: " ,sys.platform
if sys.platform == 'linux' or sys.platform == 'linux2':

   include_dirs = ['/usr/include/glib-2.0','/usr/lib/glib-2.0/include','/usr/local/include','../libGaze/include']
   libraries = ['m', 'gaze','parse_conf', 'gaze', 'glib-2.0', 'gmodule-2.0','gsl','gslcblas','gthread-2.0']
   library_dirs = ['/usr/local/lib','../libGaze/lib']
   extra_link_args = []
elif sys.platform == 'win32':

  include_dirs = ['C:/Python25/include','C:/msys/MinGW/include','C:/msys/MinGW/include/glib-2.0','C:/msys/MinGW/lib/glib-2.0/include','../libGaze/include']
  libraries = ['m', 'gaze','parse_conf', 'gaze', 'glib-2.0', 'gmodule-2.0','gsl','gslcblas','gthread-2.0']
  library_dirs = ['C:/msys/MinGW/lib','../libGaze/lib']
  extra_link_args = []#['-static']

    

setup(
  
  name = "PyGaze",
  version = '1.0-rc1',
  description='Python API for the libGaze gaze tracking framework.',
  author='Sebastian Herholz',
  author_email='Sebastian.Herholz@googlemail.com',
  url='http://sourceforge.net/projects/libgaze',
  ext_modules=[ 
  Extension("pyGaze", ["pyGaze.pyx","pyGaze_visualisation.c"],
  include_dirs = include_dirs,
  libraries = libraries,
  library_dirs = library_dirs,
  extra_link_args = extra_link_args,)
  ],
  py_modules = ['pyGazeExperiment','pyGazeGdfParser'],
  cmdclass = {'build_ext': build_ext}
)