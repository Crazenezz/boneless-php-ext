PHP_ARG_ENABLE(boneless, for boneless support,
[ --enable-boneless   Enable Boneless support])
if test "$PHP_BONELESS" = "yes"; then
  AC_DEFINE(HAVE_BONELESS, 1, [Whether you have Boneless])
  PHP_NEW_EXTENSION(boneless, boneless.c, $ext_shared)
fi
