call conda activate test
for /f "tokens=*" %%a in ('python -c "from distutils.sysconfig import get_python_lib; print(get_python_lib())"') do set SITE_PACKAGES=%%a
set CMAKE_PREFIX_PATH=%CONDA_PREFIX%/Library;%SITE_PACKAGES%;%CMAKE_PREFIX_PATH%
