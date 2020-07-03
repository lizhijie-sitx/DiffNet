from setuptools import Extension
from setuptools import setup

src = [
    "c_extension/sumdR2.cpp",
]

module = Extension("sumdR2_C",
    sources=src,
    extra_compile_args=['-std=c++11'],
    libraries=['mkl_rt'], 
    library_dirs=[],
    define_macros=[('BUILD_PYMODULE','')]
)

setup(
    name="sumdR2_C",
    version="1.0.0",
    description="DiffNet A-optimize C extension module",
    url="https://github.com/stxinsite/NetBFE",
    author="Zhijie Li",
    author_email="zhijie.li@silicontx.com",
    classifiers=[
        "Programming Language :: Python :: 2",
        "Programming Language :: Python :: 3",
    ],
    ext_modules=[module],
    test_suite="tests",
)
