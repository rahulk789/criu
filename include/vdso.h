#ifndef __CR_VDSO_H__
#define __CR_VDSO_H__

#include <sys/mman.h>

#include "asm/vdso.h"
#include "asm/int.h"

#define VDSO_PROT		(PROT_READ | PROT_EXEC)

/*
 * This is a minimal amount of symbols
 * we should support at the moment.
 */
enum {
	VDSO_SYMBOL_GETTIMEOFDAY,
	VDSO_SYMBOL_GETCPU,
	VDSO_SYMBOL_CLOCK_GETTIME,
	VDSO_SYMBOL_TIME,

	VDSO_SYMBOL_MAX
};

#define VDSO_SYMBOL_GETTIMEOFDAY_NAME	"__vdso_gettimeofday"
#define VDSO_SYMBOL_GETCPU_NAME		"__vdso_getcpu"
#define VDSO_SYMBOL_CLOCK_GETTIME_NAME	"__vdso_clock_gettime"
#define VDSO_SYMBOL_TIME_NAME		"__vdso_time"

#define VDSO_BAD_ADDR		(-1ul)
#define VDSO_BAD_PFN		(-1ull)

struct vdso_symbol {
	char		name[32];
	unsigned long	offset;
};

#define VDSO_SYMBOL_INIT						\
	{ .offset = VDSO_BAD_ADDR, }

/* Check if symbol present in symtable */
static inline bool vdso_symbol_empty(struct vdso_symbol *s)
{
	return s->offset == VDSO_BAD_ADDR && s->name[0] == '\0';
}

struct vdso_symtable {
	unsigned long		vma_start;
	unsigned long		vma_end;
	struct vdso_symbol	symbols[VDSO_SYMBOL_MAX];
};

#define VDSO_SYMTABLE_INIT						\
	{								\
		.vma_start	= VDSO_BAD_ADDR,			\
		.vma_end	= VDSO_BAD_ADDR,			\
		.symbols		= {				\
			[0 ... VDSO_SYMBOL_MAX - 1] =			\
				(struct vdso_symbol)VDSO_SYMBOL_INIT,	\
			},						\
	}

#define VDSO_INIT_SYMTABLE(symtable)					\
	*(symtable) = (struct vdso_symtable)VDSO_SYMTABLE_INIT

/* Size of VMA associated with vdso */
static inline unsigned long vdso_vma_size(struct vdso_symtable *t)
{
	return t->vma_end - t->vma_start;
}

extern struct vdso_symtable vdso_sym_rt;
extern u64 vdso_pfn;
extern int vdso_init(void);

#endif /* __CR_VDSO_H__ */
