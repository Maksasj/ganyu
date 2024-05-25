#include "ganyu_app.h"

int main() {
    GanyuApp app;

    ganyu_init(&app, "properties.ganyu.txt");
    ganyu_start(&app);
    ganyu_exit(&app);

    return 0;
}
