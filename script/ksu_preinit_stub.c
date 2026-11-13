/*
 * Preinit symbol stubs for SukiSU-Ultra / BakaSU builds where the preinit
 * runner is disabled (i.e. $(objtree)/usr/include/linux/unistd.h missing).
 *
 * The kernel-side hooks injected into fs/exec.c reference these two symbols
 * unconditionally, while the KSU build only compiles preinit/preinit.c when
 * the uapi headers are available. Without a real out-of-line definition
 * vmlinux fails to link with:
 *   ld.lld: error: undefined symbol: ksu_preinit_handle_post_execveat
 *   ld.lld: error: undefined symbol: ksu_setup_preinit_file
 *
 * Returning -EINVAL / 0 makes execve() take the normal (non-preinit) path.
 *
 * This file must stay self-contained (no preinit.h include): preinit.h
 * provides static inline versions of these helpers for KSU_NO_PREINIT, and
 * including it here would collide with the out-of-line definitions below.
 */
#ifdef KSU_NO_PREINIT

#include <linux/errno.h>

struct filename;
struct user_arg_ptr;

int ksu_setup_preinit_file(struct filename **filename_ptr, struct user_arg_ptr *argv_user);
int ksu_preinit_handle_post_execveat(int *fd, struct filename **filename_ptr, void *argv_user,
				     void *envp_user, int *flags, int *retval,
				     int *preinit_fd);

int ksu_setup_preinit_file(struct filename **filename_ptr, struct user_arg_ptr *argv_user)
{
	return -EINVAL;
}

int ksu_preinit_handle_post_execveat(int *fd, struct filename **filename_ptr, void *argv_user,
				     void *envp_user, int *flags, int *retval,
				     int *preinit_fd)
{
	return 0;
}

#endif /* KSU_NO_PREINIT */
