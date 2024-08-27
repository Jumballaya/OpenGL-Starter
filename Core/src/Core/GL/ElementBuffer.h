#pragma once

#include <iostream>

namespace Core {
  namespace GL {
    class ElementBuffer {
    public:
        ElementBuffer() = default;
        ~ElementBuffer() = default;

        void setup();
        void load(unsigned int size, unsigned int* data);
        void update(unsigned int size, unsigned int* data);
        void destroy();

        void bind(unsigned int bindPoint);

        unsigned int getId();

    private:
        unsigned int ebo = 0;
        unsigned int size = 0;
    };

  }

}