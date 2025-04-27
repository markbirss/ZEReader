#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "sd_card.h"
#include "epub.h"

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(epub, CONFIG_ZEREADER_LOG_LEVEL);

book_list_t *book_list = NULL;
current_book_t *current_book = NULL;

char *strdup(const char *s)
{
    size_t len = strlen(s) + 1;
    void *new = (char *)malloc(len);
    if (new == NULL)
        return NULL;
    return (char *)memcpy(new, s, len);
}

char *strndup(const char *s, size_t n)
{
    size_t len = strnlen(s, n);
    char *new = (char *)malloc(len + 1);
    if (new == NULL)
        return NULL;
    new[len] = '\0';
    return (char *)memcpy(new, s, len);
}

book_entry_t *epub_add_book_entry()
{
    book_list_t *node = (book_list_t *)malloc(sizeof(book_list_t));
    node->book = (book_entry_t *)malloc(sizeof(book_entry_t));
    node->next = NULL;

    if (book_list == NULL)
    {
        book_list = node;
        return node->book;
    }

    book_list_t *current = book_list;
    while (current->next != NULL)
    {
        current = current->next;
    }

    current->next = node;
    return node->book;
}

book_entry_t *epub_get_book_entry(uint16_t number)
{
    book_list_t *current = book_list;
    while (current != NULL)
    {
        if (current->book->number == number)
        {
            return current->book;
        }
        current = current->next;
    }
    return NULL;
}

book_list_t *epub_get_book_list()
{
    return book_list;
}

chapter_entry_t *epub_add_chapter_entry()
{
    chapter_list_t *node = (chapter_list_t *)malloc(sizeof(chapter_list_t));
    node->chapter = (chapter_entry_t *)malloc(sizeof(chapter_entry_t));
    node->next = NULL;
    node->prev = NULL;

    if (current_book->chapter_list == NULL)
    {
        current_book->chapter_list = node;
        return node->chapter;
    }

    chapter_list_t *current = current_book->chapter_list;
    while (current->next != NULL)
    {
        current = current->next;
    }

    current->next = node;
    current->next->prev = current;
    return node->chapter;
}

chapter_entry_t *epub_get_chapter_entry(uint16_t number)
{
    chapter_list_t *current = current_book->chapter_list;
    while (current != NULL)
    {
        if (current->chapter->number == number)
        {
            return current->chapter;
        }
        current = current->next;
    }
    return NULL;
}

chapter_list_t *epub_get_chapter_list()
{
    return current_book->chapter_list;
}

// This implementation is a first compromise in terms of time, effort and memory/search time optimization.
// Usually, the starting point (rootfile) of an ePub is called content.opf and located in the OEBPS
// directory. Both, the file name and the location are not required, but commonly used.
// In this case, the file can not be found with the current approach.
// The most correct approach would be searching for META-INF/container.xml which must be present to
// fulfill the ePub standard.
// It contains the relative path in the directory to the rootfile in such a format:
// <rootfile full-path="OEBPS/e9783641066314_content.opf" media-type="application/oebps-package+xml"/>
// As this would result in opening and parsing two file to get the relevant information,
// this naive approach is choosen to start with.
// https://www.w3.org/TR/epub-33/#sec-container-metainf
// https://cdn.standards.iteh.ai/samples/53255/6fd3b6154cf04b2da069a604a4bb76d8/ISO-IEC-TS-30135-1-2014.pdf
int epub_get_entry_points()
{

    uint16_t found = 0;
    int ret = 0;

    // Recursively searching content.opf files from SD card root is a memory efficiency nightmare
    // Instead, get top level folders and start searching the content.opf from there.
    char assumed_location[] = "/OEBPS/content.opf\0";
    uint16_t top_level_path_size = EPUB_FILE_LEN_MAX - strlen(assumed_location);

    size_t max_ls_content_size = EPUB_LSDIR_CHARS_MAX;
    char top_level_content[max_ls_content_size];
    ret = sd_card_list_files(NULL, top_level_content, &max_ls_content_size, true);
    if (ret != 0)
    {
        LOG_ERR("Listing top level files failed");
        return ret;
    }

    char *token = strtok(top_level_content, "\n");
    while (token)
    {
        // Remove [DIR ] and search for content.opf
        // Each top level folder should only contain one content.opf
        if (strstr(token, "[DIR ]") != 0)
        {
            // The prepending string [DIR ] <foldername> is always 7 chars long.
            // As strings in C are just pointers, we can just skip it by adding +7.
            // LOG_DBG("Found path: %s", token + 7);

            // Remove \r from top level path
            char top_level_path[top_level_path_size];
            strncpy(top_level_path, token + 7, strlen(token + 7));
            top_level_path[strlen(token + 7) - 1] = 0;

            if (strlen(top_level_path) + strlen(assumed_location) > EPUB_FILE_LEN_MAX)
            {
                LOG_DBG("Resulting path is too long!");
                return -1;
            }

            char assumed_path[EPUB_FILE_LEN_MAX] = {0};
            strncat(assumed_path, top_level_path, strlen(top_level_path));
            strncat(assumed_path, assumed_location, strlen(assumed_location));

            struct fs_file_t f_entry;
            fs_file_t_init(&f_entry);

            ret = sd_card_open(assumed_path, &f_entry);
            if (ret == 0)
            {
                LOG_DBG("Found valid book entrypoint");
                book_entry_t *book = epub_add_book_entry();
                LOG_DBG("Created, book entry, filling...");
                book->number = found;
                // File found, add it to the list
                book->entry_point = strdup(assumed_path);
                book->root_dir = strdup(top_level_path);
                sd_card_close(&f_entry);
                found++;
            }
        }
        token = strtok(NULL, "\n");
    }
    return ret;
}

char *epub_content_opf_metadata_get_element(const char *search_tag, const char *chapter_filename,
                                            size_t file_read_size)
{
    char *delim = ">";
    char read_buffer[file_read_size];
    uint16_t len_search_tag = strlen(search_tag) + 2;

    sd_card_open_read_close(chapter_filename, read_buffer, &file_read_size);

    char *token = strtok(read_buffer, delim);
    while (token)
    {
        if (strstr(token, search_tag) != 0)
        {
            token = strtok(NULL, delim);
            token[strlen(token) - len_search_tag] = 0;
            return token;
        }

        token = strtok(NULL, delim);
    }
}

void epub_get_authors_and_titles()
{
    size_t file_read_size = 800;
    const char *search_creator = "dc:creator";
    const char *search_title = "dc:title";
    char *element;

    book_list_t *current_elem = book_list;
    while (current_elem != NULL)
    {
        book_entry_t *book = current_elem->book;
        element = epub_content_opf_metadata_get_element(search_creator, book->entry_point, file_read_size);
        book->author = strdup(element);

        element = epub_content_opf_metadata_get_element(search_title, book->entry_point, file_read_size);
        book->title = strdup(element);
        current_elem = current_elem->next;
    }
}

int char_get_index(char *str, char c)
{
    char *end = strchr(str, c);
    if (end == NULL)
    {
        return -1;
    }
    return (int)(end - str);
}

int epub_parse_chapter_files(const char *content_opf)
{
    int ret = 0;
    size_t offset = 0;
    size_t read_size = 800;
    char buf[read_size];
    char *delim = ">";
    char *search_tag = "href=\"";
    uint16_t len_search_tag = strlen(search_tag);

    while (read_size == 800)
    {
        ret = sd_card_open_read_at_offset_close(content_opf, &offset, buf, &read_size);
        if (ret)
        {
            LOG_ERR("Could parse chapters!");
            return ret;
        }

        char *token = strtok(buf, delim);
        while (token)
        {
            char *found_href = strstr(token, search_tag);
            if (found_href != 0)
            {
                // href="Text/BD0B5209B23149EC818E6FE711A195F6.xhtml" id="x978-3-641-16998-5-36" media-type="application/xhtml+xml"
                // By adding the length of href=", we get our start pointer
                char *path = found_href + len_search_tag;

                // The next occurrence of " is the end of the path
                int path_len = char_get_index(path, '"');

                if (path_len >= EPUB_FILE_LEN_MAX)
                {
                    LOG_ERR("Found path is too long");
                    return -1;
                }
                else if (path_len < EPUB_FILE_LEN_MAX && path_len > 0)
                {
                    chapter_entry_t *chapter = epub_add_chapter_entry();
                    chapter->path = (char *)malloc((sizeof(current_book->root_dir) + 7 + path_len) * sizeof(char));
                    strcpy(chapter->path, current_book->root_dir);
                    strcat(chapter->path, "/OEBPS/");
                    strncat(chapter->path, path, path_len);
                    chapter->number = current_book->num_chapters;

                    LOG_DBG("chapter path: %s", chapter->path);
                    current_book->num_chapters++;
                }
                else if (path_len < 0)
                {
                    // Path len is negative, current read batch is finished
                    // We need to move the offset back to the start of this href to make sure we find it
                    offset -= strlen(token);
                }
            }

            token = strtok(NULL, delim);
        }
    }
    return ret;
}

bool is_html(char *path)
{
    return strstr(path, "html");
}

int epub_get_next_chapter()
{
    current_book->state.file_offset = 0;
    LOG_DBG("Get next chapter");
    if (current_book->state.chapter == current_book->num_chapters - 1)
    {
        LOG_DBG("Book finished!");
        return 0;
    }
    if (current_book->current_chapter == NULL)
    {
        // Set linked list pointer to the first element
        current_book->current_chapter = current_book->chapter_list;
    }

    if (current_book->current_chapter->next != NULL)
    {
        current_book->current_chapter = current_book->current_chapter->next;
        current_book->state.chapter++;

        if (!is_html(current_book->current_chapter->chapter->path))
        {
            epub_get_next_chapter();
        }
    }
}

int epub_get_prev_chapter()
{
    if (current_book->state.chapter == 0)
    {
        LOG_DBG("Already at the beginning");
        return 0;
    }

    if (current_book->current_chapter->next != NULL)
    {
        current_book->current_chapter = current_book->current_chapter->prev;
        current_book->state.chapter--;

        if (!is_html(current_book->current_chapter->chapter->path))
        {
            epub_get_prev_chapter();
        }
        else
        {
            // TODO does not work as expected right now
            LOG_DBG("Opening previous file %s", current_book->current_chapter->chapter->path);
            sd_card_tell_end_offset(current_book->current_chapter->chapter->path, &current_book->state.file_offset);
            LOG_DBG("End offset of the prev chapter: %d", current_book->state.file_offset);
            current_book->state.file_offset -= (EPUB_PAGE_SIZE - 1);
        }
    }
}

int epub_parse_next_page()
{
    size_t read_size = (EPUB_PAGE_SIZE - 1);
    LOG_DBG("Current offset: %d", current_book->state.file_offset);

    int ret = sd_card_open_read_at_offset_close(current_book->current_chapter->chapter->path, &current_book->state.file_offset,
                                                current_book->page, &read_size);
    current_book->page[read_size] = 0;

    LOG_DBG("%s", current_book->page);

    if (read_size < (EPUB_PAGE_SIZE - 1))
    {
        LOG_DBG("Open next chapter");
        epub_get_next_chapter();
    }

    return ret;
}

char *epub_get_next_page()
{

    if (epub_parse_next_page() == 0)
    {
        return current_book->page;
    }
    LOG_DBG("Can't get page");
    return "";
}

int epub_parse_prev_page()
{
    size_t read_size = (EPUB_PAGE_SIZE - 1);
    if (current_book->state.file_offset > (2 * read_size))
    {
        current_book->state.file_offset -= 2 * read_size;
    }
    else if (current_book->state.file_offset == read_size)
    {
        LOG_DBG("Open previous chapter");
        epub_get_prev_chapter();
    }
    else
    {
        current_book->state.file_offset = 0;
    }

    LOG_DBG("Opening prev page at offset :%d", current_book->state.file_offset);

    int ret = sd_card_open_read_at_offset_close(current_book->current_chapter->chapter->path, &current_book->state.file_offset,
                                                current_book->page, &read_size);
    current_book->page[read_size] = 0;
    LOG_DBG("%s", current_book->page);

    // if (ret < 0)
    // {
    //     LOG_DBG("Open previous chapter");
    //     epub_get_prev_chapter();
    // }

    return ret;
}

char *epub_get_prev_page()
{

    if (epub_parse_prev_page() == 0)
    {
        return current_book->page;
    }
    LOG_DBG("Can't get page");
    return "";
}

int epub_open_book(book_entry_t *book)
{
    free(current_book);
    current_book = (current_book_t *)malloc(sizeof(current_book_t));

    current_book->state.title = strdup(book->title);
    current_book->state.chapter = -1;
    current_book->state.file_offset = 0;

    current_book->num_chapters = 0;
    current_book->chapter_list = NULL;
    current_book->current_chapter = NULL;
    current_book->chapter_filename = NULL;
    current_book->root_dir = book->root_dir;

    epub_parse_chapter_files(book->entry_point);
    epub_get_next_chapter();
}

int epub_initialize()
{
    int ret = 0;

    LOG_DBG("Init SD card");
    ret = sd_card_init();
    if (ret)
    {
        LOG_ERR("Initializing the SD card failed");
        return ret;
    }

    LOG_DBG("Get entry points");
    // ret =
    epub_get_entry_points();
    // if (ret) {
    //     LOG_ERR("Parse EBOOK entry points failed");
    //     return ret;
    // }

    LOG_DBG("Get authors and titles");
    // ret =
    epub_get_authors_and_titles();
    // if (ret) {
    //     LOG_ERR("Parse book titles failed");
    //     return ret;
    // }
}