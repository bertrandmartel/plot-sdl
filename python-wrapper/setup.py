from distutils.core import setup
from distutils.extension import Extension
#from Cython.Build import cythonize
from Cython.Distutils import build_ext

ext_modules = [
Extension(
	name="plotsdl.plot",
	sources=["plotsdl/plot.pyx"],
	include_dirs = ["../release/include"],
	libraries=["plotsdl","m","SDL2","SDL2_ttf"],
	library_dirs=["../release","/usr/local/lib/","/usr/lib"],
	language="c",)
]

setup(
	name = 'plotsdl',
	cmdclass = {'build_ext': build_ext},
	ext_modules = ext_modules
)