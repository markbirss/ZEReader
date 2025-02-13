#ifndef _EPUB_H_
#define _EPUB_H_

#define EPUB_FILE_NUM_MAX 6
#define EPUB_FILE_LEN_MAX 100
#define EPUB_LSDIR_CHARS_MAX 2000
#define EPUB_TITLE_AUTHOR_LEN_MAX 50
#define EPUB_CHAPTERS_MAX 150
#define EPUB_PAGE_SIZE 300

typedef struct {
    size_t number;
    char *title;
    char *author;
    char *entry_point;
    char *root_dir;
} book_entry_t;

typedef struct {
    uint16_t num_chapters;
    char chapter_list[EPUB_CHAPTERS_MAX][EPUB_FILE_LEN_MAX];
    char page[EPUB_PAGE_SIZE];
    char chapter_filename[EPUB_FILE_LEN_MAX];
    char *root_dir;

    struct {
        // May use title instead of number in future
        char *title;
        //size_t book_number;
        size_t chapter;
        size_t file_offset;
    } state;

} current_book_t;

typedef struct llist {
    book_entry_t *book;
    struct llist *next;
} book_list_t;



int epub_initialize();

book_list_t *epub_get_book_list();

book_entry_t *epub_get_book_entry(uint16_t number);
int epub_open_book(book_entry_t *book);

char* epub_get_prev_page();
char* epub_get_next_page();


#endif