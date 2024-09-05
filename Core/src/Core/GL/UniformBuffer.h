#pragma once

#include <glad/glad.h>

namespace Core {
  namespace GL {

    template <class T>
    class UniformBuffer {
    public:
        UniformBuffer(unsigned int idx = 0) : index(idx) {};
        ~UniformBuffer() {
            if (buffer == 0) return;
            glDeleteBuffers(1, &buffer);
        };

        // Move
        UniformBuffer(UniformBuffer&& other) noexcept {
            buffer = other.buffer;
            index = other.index;
            other.buffer = 0;
        }

        // Move
        UniformBuffer& operator=(UniformBuffer&& other) noexcept {
            buffer = other.buffer;
            index = other.index;
            other.buffer = 0;
            return *this;
        }

        void initialize() {
            glCreateBuffers(1, &buffer);
        }

        void load(unsigned int idx) {
            index = idx;
            glNamedBufferStorage(buffer, sizeof(T), nullptr, GL_DYNAMIC_STORAGE_BIT);
            glBindBufferRange(GL_UNIFORM_BUFFER, index, buffer, 0, sizeof(T));
        }

        void update(T data) {
            glNamedBufferSubData(buffer, 0, sizeof(T), &data);
        }

        void bind(unsigned int idx = index) {
            if (idx != index) index = idx;
            glBindBufferRange(GL_UNIFORM_BUFFER, index, buffer, 0, sizeof(T));
        }

        GLuint getId() { return buffer; }

        void unbind() {} // dummy function for API completeness for now

    private:
        GLuint buffer = 0;
        unsigned int index;
    };

  }

}