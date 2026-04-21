#ifndef QT_ARM_ACLE_WORKAROUND_H
#define QT_ARM_ACLE_WORKAROUND_H

#if defined(__aarch64__) && defined(__has_include)
#  if __has_include(<arm_acle.h>)
#    include <arm_acle.h>
#  endif
#endif

#endif // QT_ARM_ACLE_WORKAROUND_H
