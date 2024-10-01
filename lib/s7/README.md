# s7 - a small scheme interpreter

https://ccrma.stanford.edu/software/snd/snd/s7.html

```cpp
#include "s7.h"

class App : public Imp::Main {
public:
    App() : Imp::Main("Imp", WINDOW_SIZE) {
        s7 = s7_init();
    }

    ~App() {
        s7_quit(s7);
    }

    void evalScheme(const std::string& expr) {
        s7_eval_c_string(s7, expr.c_str());
    }

    void render(Graphics* graph) override {
        graph->setColor(Color(255, 255, 255));
        graph->text("Hello World", Vec2i(20, 20));
        graph->text(std::to_string(graph->getTick()), Vec2i(20, 40));
        graph->sprite(Vec2i(100, 100), Vec2i(32, 32), Vec2i(0, 0), Vec2i(32, 32));

        // Example usage of the Scheme interpreter
        evalScheme("(display \"Hello from Scheme!\")");
    }

private:
    s7_scheme* s7;
};
```

```bash
gcc -c ./src/s7.c -o ./src/s7.o -std=c11
g++ ./src/main.cpp ./src/s7.o -o ./out/main.app 
```