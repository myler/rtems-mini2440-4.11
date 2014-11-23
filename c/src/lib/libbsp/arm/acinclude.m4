# RTEMS_CHECK_BSPDIR(RTEMS_BSP_FAMILY)
AC_DEFUN([RTEMS_CHECK_BSPDIR],
[
  case "$1" in
  mini2440 )
    AC_CONFIG_SUBDIRS([mini2440]);;
  *)
    AC_MSG_ERROR([Invalid BSP]);;
  esac
])
