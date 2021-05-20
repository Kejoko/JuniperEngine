#pragma once

#include <fstream>
#include <string>
#include <vector>

namespace jun {
    std::vector<char> readCompiledShader(const std::string&);
}

std::vector<char> jun::readCompiledShader(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        jun::Logger::critical("Failed to open file: " + filename);
        throw std::runtime_error("Failed to open file: " + filename);
    }

    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}