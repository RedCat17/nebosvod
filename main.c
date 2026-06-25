#include <pthread.h>
#include "app.h"

int main() {
    AppContext app_ctx = {0};

    init_app(&app_ctx);

    start_app(&app_ctx);

    return 0;
}