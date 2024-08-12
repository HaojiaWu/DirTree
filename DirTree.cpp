#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <iomanip>

namespace fs = std::filesystem;

std::string formatFileSize(uintmax_t size) {
    const uintmax_t KB = 1024;
    const uintmax_t MB = KB * 1024;
    const uintmax_t GB = MB * 1024;

    std::ostringstream oss;

    if (size >= GB) {
        oss << std::fixed << std::setprecision(2) << static_cast<double>(size) / GB << " GB";
    } else if (size >= MB) {
        oss << std::fixed << std::setprecision(2) << static_cast<double>(size) / MB << " MB";
    } else if (size >= KB) {
        oss << std::fixed << std::setprecision(2) << static_cast<double>(size) / KB << " KB";
    } else {
        oss << size << " bytes";
    }

    return oss.str();
}

void generateHTML(const fs::path& path, std::ofstream& outFile, int depth = 0, bool isTopLevel = false) {
    std::string indent(depth * 20, ' ');

    if (fs::is_directory(path)) {
        outFile << indent << "<li>" << std::endl;
        outFile << indent << "  <details>" << std::endl;

        std::string displayName = isTopLevel ? path.string() : path.filename().string();

        outFile << indent << "    <summary>" << displayName << " (Directory)" << "</summary>" << std::endl;
        outFile << indent << "    <ul>" << std::endl;

        for (const auto& entry : fs::directory_iterator(path)) {
            generateHTML(entry.path(), outFile, depth + 1);
        }

        outFile << indent << "    </ul>" << std::endl;
        outFile << indent << "  </details>" << std::endl;
        outFile << indent << "</li>" << std::endl;
    } else if (fs::is_regular_file(path)) {
        outFile << indent << "<li>" << path.filename().string() << " ("
                << "<strong>" << formatFileSize(fs::file_size(path)) << "</strong>)" << "</li>" << std::endl;
    }
}

int main(int argc, char* argv[]) {
    std::string outputFileName = "directory_structure.html";
    std::string directoryPath;

    // Parse command-line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if ((arg == "-o" || arg == "--output") && i + 1 < argc) {
            outputFileName = argv[++i];
        } else if (directoryPath.empty()) {
            directoryPath = arg;
        } else {
            std::cerr << "Unknown argument: " << arg << std::endl;
            return 1;
        }
    }

    if (directoryPath.empty()) {
        std::cerr << "Usage: " << argv[0] << " [/path/to/directory] [-o output_file.html]" << std::endl;
        return 1;
    }

    if (!fs::exists(directoryPath) || !fs::is_directory(directoryPath)) {
        std::cerr << "Error: The path does not exist or is not a directory." << std::endl;
        return 1;
    }

    std::ofstream outFile(outputFileName);
    if (!outFile) {
        std::cerr << "Error: Could not create the output file." << std::endl;
        return 1;
    }

    // HTML header, CSS, and JavaScript
    outFile << "<!DOCTYPE html>" << std::endl;
    outFile << "<html>" << std::endl;
    outFile << "<head><title>Directory Structure of " << directoryPath << "</title>" << std::endl;
    outFile << "<style>" << std::endl;
    outFile << "body { font-family: Arial, sans-serif; background-color: #f4f4f4; }" << std::endl;
    outFile << "ul { list-style-type: none; margin: 0; padding: 0; }" << std::endl;
    outFile << "li { margin: 5px 0; padding-left: 20px; }" << std::endl;
    outFile << "details summary { cursor: pointer; font-weight: bold; }" << std::endl;
    outFile << "details[open] summary { color: #1e90ff; }" << std::endl;
    outFile << "details ul { margin-left: 20px; padding-left: 20px; border-left: 2px dashed #ccc; }" << std::endl;
    outFile << "li p { display: inline; }" << std::endl;
    outFile << "</style>" << std::endl;
    outFile << "</head>" << std::endl;
    outFile << "<body>" << std::endl;
    outFile << "<h1>Directory Structure of " << directoryPath << "</h1>" << std::endl;
    outFile << "<ul>" << std::endl;

    generateHTML(directoryPath, outFile, 0, true);

    outFile << "</ul>" << std::endl;
    outFile << "</body>" << std::endl;
    outFile << "</html>" << std::endl;

    outFile.close();

    std::cout << "HTML file generated: " << outputFileName << std::endl;
    return 0;
}
