#include "../largebuffer.h"

int main() {
    LargeBuffer buffer;
    std::string str("hel\nlo");

    if (buffer.Avail() > str.size()) {
        buffer.Append(str);
    }
    std::string file_name("test1.txt");
    buffer.StoreToFile(file_name);
    buffer.SetBufferZero();
}

