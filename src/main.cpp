/*
 * Copyright 2011 The Emscripten Authors.  All rights reserved.
 * Emscripten is available under two separate licenses, the MIT license and the
 * University of Illinois/NCSA Open Source License.  Both these licenses can be
 * found in the LICENSE file.
 */

#include <iostream>

#include "Eso.h"

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#endif

App app;

void main_loop() {
    app.loop();
    if (app.shouldQuit) {
        DBG("Main quitting");
        #ifdef __EMSCRIPTEN__
            emscripten_cancel_main_loop();
        #else
            exit(0);
        #endif
    }
}

int main(int argc, char* argv[]) {
    std::cout << "MAIN Startup" << std::endl;
    #ifdef __EMSCRIPTEN__
        DBG("Emscripten main loop");
        emscripten_set_main_loop(main_loop, 0, 1);
        return 0;
    #else
        DBG("Standard main loop");
    #endif
    while (true) {
        main_loop();
    }
    return 0;
}
