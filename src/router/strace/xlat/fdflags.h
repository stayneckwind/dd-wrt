/* Generated by ./xlat/gen.sh from ./xlat/fdflags.in; do not edit. */

#include "gcc_compat.h"
#include "static_assert.h"


#ifndef XLAT_MACROS_ONLY

# ifdef IN_MPERS

#  error static const struct xlat fdflags in mpers mode

# else

static const struct xlat_data fdflags_xdata[] = {
#if defined(FD_CLOEXEC) || (defined(HAVE_DECL_FD_CLOEXEC) && HAVE_DECL_FD_CLOEXEC)
  XLAT(FD_CLOEXEC),
#endif
};
static
const struct xlat fdflags[1] = { {
 .data = fdflags_xdata,
 .size = ARRAY_SIZE(fdflags_xdata),
 .type = XT_NORMAL,
} };

# endif /* !IN_MPERS */

#endif /* !XLAT_MACROS_ONLY */
