#ifndef _EPUB_H_
#define _EPUB_H_

#define EPUB_FILE_NUM_MAX 6
#define EPUB_FILE_LEN_MAX 100
#define EPUB_LSDIR_CHARS_MAX 2000
#define EPUB_TITLE_AUTHOR_LEN_MAX 50
#define EPUB_CHAPTERS_MAX 150
#define EPUB_PAGE_SIZE 300

int epub_initialize();


char (*epub_get_title_list(void))[EPUB_TITLE_AUTHOR_LEN_MAX];
char (*epub_get_author_list(void))[EPUB_TITLE_AUTHOR_LEN_MAX];

int epub_open_book(uint16_t num);

char* epub_get_prev_page();
char* epub_get_next_page();

// int epub_get_entry_points();
// void epub_parse_book_titles();
// void epub_parse_book_authors();
// int epub_parse_next_page();
// int epub_parse_prev_page();

#endif