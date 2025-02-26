conda activate test
SITE_PACKAGES=$(python -c "from distutils.sysconfig import get_python_lib; print(get_python_lib())")
export CMAKE_PREFIX_PATH=$CONDA_PREFIX:$SITE_PACKAGES:$CMAKE_PREFIX_PATH
