#include "main_window.h"

int main(int argc, char **argv) {
    MainWindow window = MainWindow_create();
    MainWindow_run(&window);
    MainWindow_destroy(&window);

    return 0;
}
