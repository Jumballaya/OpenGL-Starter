#pragma once

#include <iostream>

namespace Core {
  namespace GL {

    template <class T>
    class UniformBuffer {
    public:
        UniformBuffer(unsigned int idx = 0) : index(idx) {};

        ~UniformBuffer() {
            glDeleteBuffers(1, &buffer);
        }

        void setup() {
            glCreateBuffers(1, &buffer);
            glNamedBufferStorage(buffer, sizeof(T), nullptr, GL_DYNAMIC_STORAGE_BIT);
            glBindBufferRange(GL_UNIFORM_BUFFER, index, buffer, 0, sizeof(T));
        }

        void setData(T& data) {
            glNamedBufferSubData(buffer, 0, sizeof(T), data);
        }

        void bind(unsigned int idx) {
            index = idx;
            glBindBufferRange(GL_UNIFORM_BUFFER, index, buffer, 0, sizeof(T));
        }

    private:
        unsigned int buffer = 0;
        unsigned int index;
    };

  }

}