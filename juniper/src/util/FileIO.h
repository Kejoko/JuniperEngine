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
        std::string openFailureString = "Failed to open shader file: " + filename;
        jun::Logger::critical(openFailureString);
        throw std::runtime_error(openFailureString);
    }

    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}