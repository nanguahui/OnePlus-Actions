/*
 * Preinit symbol stubs for SukiSU-Ultra builds where the preinit runner is
 * disabled (i.e. $(objtree)/usr/include/linux/unistd.h does not exist).
 *
 * The kernel-side hooks injected into fs/exec.c reference these two symbols
 * unconditionally, while drivers/kernelsu/Makefile only compiles
 * preinit/preinit.o when the uapi headers are available. Without these
 * no-op implementations vmlinux fails to link with:
 *   ld.lld: error: undefined symbol: ksu_preinit_handle_post_execveat
 *   ld.lld: error: undefined symbol: ksu_setup_preinit_file
 *
 * Returning -EINVAL / 0 makes execve() take the normal (non-preinit) path.
 */
#ifdef KSU_NO_PREINIT

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
