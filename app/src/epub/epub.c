#include <string.h>
#include <stdio.h>

#include "sd_card.h"

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(epub, LOG_LEVEL_DBG);

#define EPUB_MAX_LEVEL_CONTENT 1000
#define EPUB_FILE_NUM_MAX 10
#define EPUB_FILE_LEN_MAX 100
#define EPUB_TITLE_AUTHOR_LEN_MAX 20

static char epub_entry_points[EPUB_FILE_NUM_MAX][EPUB_FILE_LEN_MAX];
// static char epub_titles[EPUB_FILE_NUM_MAX][EPUB_TITLE_AUTHOR_LEN_MAX];
// static char epub_authors[EPUB_FILE_NUM_MAX][EPUB_TITLE_AUTHOR_LEN_MAX];
static uint16_t found_books = 0;

int epub_init_sd_card()
{
    return sd_card_init();
}

// This implementation is a first compromise in terms of time, effort and memory/search time optimization.
// Usually, the starting point (rootfile) of an ePub is called content.opf and located in the OEBPS directory.
// Both, the file name and the location are not required, but commonly used.
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

    size_t max_level_content_size = EPUB_MAX_LEVEL_CONTENT;
    char top_level_content[max_level_content_size];
    ret = sd_card_list_files(NULL, top_level_content, &max_level_content_size, true);
    if (ret != 0)
    {
        LOG_DBG("Listing top level files failed");
        return ret;
    }

    LOG_DBG("Top Level Content: %s", top_level_content);

    char *token = strtok(top_level_content, "\n");
    while (token)
    {
        // // Recursive search approach - kept here for reference
        // // Works but still inefficient and issues with large folders
        // ret = sd_card_list_files_match(1, EPUB_FILE_LEN_MAX, content_opf, token+7, "content.opf");
        // if (ret > 0) {
        //     LOG_DBG("Found: %s", content_opf[0]);
        //     // Found file, copy it to the permanent buffer
        //     strcpy(epub_entry_points[found], content_opf[0]);
        //     found++;
        // }

        // Remove [DIR ] and search for content.opf
        // Each top level folder should only contain one content.opf
        if (strstr(token, "[DIR ]") != 0)
        {
            // The prepending string [DIR ] <foldername> is always 7 chars long.
            // As strings in C are just pointers, we can just skip it by adding +7.
            LOG_DBG("Found path: %s", token + 7);

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
            LOG_DBG("Assumed path %s", assumed_path);

            struct fs_file_t f_entry;
            fs_file_t_init(&f_entry);

            ret = sd_card_open(assumed_path, &f_entry);
            if (ret == 0)
            {
                LOG_DBG("Found %s", assumed_path);
                // File found, add it to the list
                strcpy(epub_entry_points[found], assumed_path);
                sd_card_close(&f_entry);
                found++;
            }
        }
        token = strtok(NULL, "\n");
    }

    for (size_t i = 0; i < found; i++)
    {
        LOG_DBG("Entry point: %s", epub_entry_points[i]);
    }

    found_books = found;
    return found;
}

// size_t read_buffer_size = 2000;
// char read_buffer[read_buffer_size];
// // OEBPS/Text/755B3C1FD5C84C238E2AFE25B22BBF61.xhtml
// // OEBPS/Text/875D1C7E2F4549079666C018192CD2B0.xhtml
// sd_card_open_read_close("Flavia/OEBPS/Text/755B3C1FD5C84C238E2AFE25B22BBF61.xhtml", read_buffer, &read_buffer_size);
// // sd_card_open_read_close("FLAVIA/test.xhtml", read_buffer, &read_buffer_size);
// read_buffer[read_buffer_size] = '\0';
// LOG_DBG("%s", read_buffer);

int epub_get_title_and_author()
{
    for (size_t i = 0; i < found_books; i++)
    {
    }
}
