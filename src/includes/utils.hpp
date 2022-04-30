#pragma once
namespace Mer {
    namespace Utils {
        template <typename T>
        void buffer_write(char* buffer, size_t pos, const T& val)
        {
            *(T*)(buffer + pos) = val;
        }

        template <typename T>
        T& buffer_read(char* buffer, size_t pos)
        {
            return *(T*)(buffer + pos);
        }
        template <typename T>
        void buffer_write(char* buffer, const T& val)
        {
            *(T*)(buffer) = val;
        }
        template <typename T>
        T& buffer_read(char* buffer)
        {
            return *(T*)(buffer);
        }
    }
}