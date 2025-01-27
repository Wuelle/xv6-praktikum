/*! \file user.h
 * \brief header for userspace standard library
 */

#ifndef INCLUDED_user_user_h
#define INCLUDED_user_user_h

#ifdef __cplusplus
extern "C" {
#endif

#include "kernel/stat.h"



// system calls
int fork(void);
int exit(int) __attribute__((noreturn));
int wait(int*);
int pipe(int*);
int write(int, const void*, int) __attribute__((weak));
int read(int, void*, int);
int close(int);
int kill(int);
int exec(const char*, char**);
int open(const char*, int);
int mknod(const char*, short, short);
int unlink(const char*);
int fstat(int fd, struct stat*);
int link(const char*, const char*);
int mkdir(const char*);
int chdir(const char*);
int dup(int);
int getpid(void);
char* sbrk(int);
int sleep(int);
int uptime(void);
void cxx(int);
void term();
int hello_kernel(int);
int printPT(void);
int futex_init(uint64* futex);
int futex_wait(uint64* futex, uint64 val);
int futex_wake(uint64* futex, int num_wake);
int net_test(void);
int net_send_listen(uint8 id, void* send_buffer, int send_buffer_length, void* receive_buffer, int receive_buffer_length);
int net_bind(uint16 port);
void net_unbind(int id);

// ulib.c
int stat(const char*, struct stat*);
char* strcpy(char*, const char*);
void *memmove(void*, const void*, int);
char* strchr(const char*, char c);
int strcmp(const char*, const char*);
void fprintf(int, const char*, ...);
void printf(const char*, ...);
char* gets(char*, int max);
uint strlen(const char*);
void* memset(void*, int, uint);
void* malloc(uint);
void setup_malloc();
void free(void*);
int atoi(const char*);
int memcmp(const void *, const void *, uint);
void *memcpy(void *, const void *, uint);



#ifdef __cplusplus
}
#endif

#endif
