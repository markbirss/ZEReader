#ifndef _SD_H_
#define _SD_H_

#include <stddef.h>
#include <zephyr/fs/fs.h>

int sd_initialize(void);

int sd_open(char const *const filename, struct fs_file_t *f_obj);

int sd_close(struct fs_file_t *f_obj);

int sd_read(struct fs_file_t *f_obj, char *buffer, size_t *size);

int sd_read_chunk(char const *const filename, size_t *offset, char *const buffer, size_t *size);

//int sd_write();
int sd_write_chunk(char const *const filename, char const *const data, size_t *size);

int sd_list_directories(char const *const search_path, char *buffer, size_t *size);

int sd_tell_end_offset(char const *const filename, size_t *offset);

#endif