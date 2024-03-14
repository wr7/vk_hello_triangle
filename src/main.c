#include "main_window.h"

int main(void) {
    MainWindow *window = MainWindow_create();
    MainWindow_run(window);
    MainWindow_destroy(window);

    return 0;
}
