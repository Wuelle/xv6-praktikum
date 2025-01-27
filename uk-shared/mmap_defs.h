/*! \file mmap_defs.h
 * \brief mmap defines
 */

#ifndef INCLUDED_shared_mmap_defs_h
#define INCLUDED_shared_mmap_defs_h

#ifdef __cplusplus
extern "C" {
#endif

#include "uk-shared/error_codes.h"

#define MMAP_MIN_ADDR (void*)((uint64)1 << 29)

#define PROT_READ   0x1
#define PROT_WRITE  0x2
#define PROT_EXEC   0x4
#define PROT_NONE   0x0

#define MAP_SHARED                0x01
#define MAP_PRIVATE               0x02
#define MAP_SHARED_VALIDATE       0x03
#define MAP_FIXED                 0x10
#define MAP_ANONYMOUS             0x20
#define MAP_32BIT                 0x40
#define MAP_DENYWRITE            0x800
#define MAP_EXECUTABLE          0x1000
#define MAP_LOCKED              0x2000
#define MAP_NORESERVE           0x4000
#define MAP_POPULATE            0x8000
#define MAP_NONBLOCK           0x10000
#define MAP_STACK              0x20000
#define MAP_HUGETLB            0x40000
#define MAP_SYNC               0x80000
#define MAP_FIXED_NOREPLACE   0x100000
#define PROT_GROWSDOWN       0x1000000
#define PROT_GROWSUP         0x2000000
#define MAP_UNINITIALIZED    0x4000000

#define MAP_ANON MAP_ANONYMOUS

#define HUGETLB_FLAG_ENCODE_SHIFT  26
#define HUGETLB_FLAG_ENCODE_2MB (21U << HUGETLB_FLAG_ENCODE_SHIFT)
#define HUGETLB_FLAG_ENCODE_1GB (30U << HUGETLB_FLAG_ENCODE_SHIFT)
#define MAP_HUGE_2MB HUGETLB_FLAG_ENCODE_2MB
#define MAP_HUGE_1GB HUGETLB_FLAG_ENCODE_1GB


#ifdef __cplusplus
}
#endif

#endif
