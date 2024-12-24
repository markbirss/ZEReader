#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/storage/disk_access.h>
#include <zephyr/logging/log.h>
#include <zephyr/fs/fs.h>

char* author;
char* title;

get_author() {

}

get_title() {

}


// Ein Buch ist in der Reihenfolge über Elemente im Manifest definiert.
// <manifest>
// Step 1 -> nur Elemente mit media-type "application/xhtml+xml" parsen und anschauen

int index_current_chapter;

get_next_chapter() {

}

get_previous_chapter() {

}

// Text Anzeigen
// Innerhalb von Kapitel xhtml files ist je ein Satz ein <p> content <p>
// -> Sätze einzeln parsen -> kann man LVGL fragen ob noch genug Zeichen in ein Textelement
// mit definierter Groeße geschrieben werden koennen?

// State: counter aktuelles element! -> Persistent!