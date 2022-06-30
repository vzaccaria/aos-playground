#include "linux/jiffies.h"
#include "linux/spinlock_types.h"
#include "linux/workqueue.h"
#include <asm/io.h>
#include <linux/circ_buf.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/wait.h>

#ifndef SERIAL_AOS_CIRCBUF_H
#define SERIAL_AOS_CIRCBUF_H

/* The following circular buffer follows the implementation suggested by Howells
 * and Mc.Kenney in
 *
 * https://www.kernel.org/doc/html/latest/core-api/circular-buffers.html
 *
 * We've macro'd out just to make it easy to implement more ringbuffers
 */

#define Q_DECL(NAME, SZ, ELSIZE)                                               \
  typedef struct {                                                             \
    char buf[SZ * ELSIZE];                                                     \
    int head;                                                                  \
    int tail;                                                                  \
  } circ_buf_##NAME;                                                           \
  circ_buf_##NAME cb_##NAME;                                                   \
  DEFINE_SPINLOCK(sl_cons_##NAME);                                             \
  DEFINE_SPINLOCK(sl_prod_##NAME);                                             \
  int sz_##NAME = SZ * ELSIZE;

#define Q_PUT(NAME, jj)                                                        \
  {                                                                            \
    unsigned long head_##NAME;                                                 \
    unsigned long tail_##NAME;                                                 \
    head_##NAME = cb_##NAME.head;                                              \
    tail_##NAME = READ_ONCE(cb_##NAME.tail);                                   \
    if (CIRC_SPACE(head_##NAME, tail_##NAME, sz_##NAME) >= 1) {                \
      cb_##NAME.buf[head_##NAME] = jj;                                         \
      smp_store_release(&cb_##NAME.head, (head_##NAME + 1) & (sz_##NAME - 1)); \
    }                                                                          \
  }

#define Q_PUT_S(NAME, jj)                                                      \
  spin_lock(&sl_prod_##NAME);                                                  \
  Q_PUT(NAME, jj)                                                              \
  spin_unlock(&sl_prod_##NAME);

#define Q_GET(NAME, var, tocons)                                               \
  {                                                                            \
    unsigned long head_##NAME;                                                 \
    unsigned long tail_##NAME;                                                 \
    head_##NAME = smp_load_acquire(&(cb_##NAME.head));                         \
    tail_##NAME = cb_##NAME.tail;                                              \
    if (CIRC_CNT(head_##NAME, tail_##NAME, sz_##NAME) >= 1) {                  \
      var = cb_##NAME.buf[tail_##NAME];                                        \
      smp_store_release(&(cb_##NAME.tail),                                     \
                        (tail_##NAME + 1) & (sz_##NAME - 1));                  \
      tocons = 1;                                                              \
    } else {                                                                   \
      tocons = 0;                                                              \
    }                                                                          \
  }

/* This is a short hack; must be checked against the LKMM, as done for the
 * previous macros. */
#define Q_CNT(NAME)                                                            \
  (CIRC_CNT(READ_ONCE(cb_##NAME.head), READ_ONCE(cb_##NAME.tail), sz_##NAME))

#define Q_GET_S(NAME, jj, tocons)                                              \
  spin_lock(&sl_cons_##NAME);                                                  \
  Q_GET(NAME, jj, tocons)                                                      \
  spin_unlock(&sl_cons_##NAME);

#endif
