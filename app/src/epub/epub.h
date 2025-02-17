#ifndef _EPUB_H_
#define _EPUB_H_

// 255 is the maximum file name length supported by the filesystem
#define EPUB_FILE_LEN_MAX 255 
#define EPUB_LSDIR_CHARS_MAX 4096
#define EPUB_PAGE_SIZE 300


typedef struct {
    size_t number;
    char *title;
    char *author;
    char *entry_point;
    char *root_dir;
} book_entry_t;

typedef struct llist {
    book_entry_t *book;
    struct llist *next;
} book_list_t;

typedef struct {
    size_t number;
    char *path;
} chapter_entry_t;

typedef struct dllist {
    chapter_entry_t *chapter;
    struct dllist *next;
    struct dllist *prev;
} chapter_list_t;

typedef struct {
    uint16_t num_chapters;
    chapter_list_t *chapter_list;
    chapter_list_t *current_chapter;
    char page[EPUB_PAGE_SIZE];
    char *chapter_filename;
    char *root_dir;

    struct {
        // May use title instead of number in future
        char *title;
        //size_t book_number;
        size_t chapter;
        size_t file_offset;
    } state;

} current_book_t;



int epub_initialize();

book_list_t *epub_get_book_list();

book_entry_t *epub_get_book_entry(uint16_t number);
int epub_open_book(book_entry_t *book);

char* epub_get_prev_page();
char* epub_get_next_page();


#endif