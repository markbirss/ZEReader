#ifndef _EPUB_H_
#define _EPUB_H_

int epub_initialize();
int epub_get_entry_points();
void epub_get_book_titles();
void epub_get_book_authors();
int epub_get_next_page();
int epub_get_prev_page();

#endif