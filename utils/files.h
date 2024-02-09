//
// Created by alexa on 28/01/2024.
//

#ifndef NOTE_APP_FILES_H
#define NOTE_APP_FILES_H

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>


/**
 * @brief Extracts nouns and their associated numbers from a file and writes them to another file.
 *
 * This function reads an input file line by line, where each line is expected to be in the format
 * "word_VALUE vector_of_that_word". It extracts words where VALUE is "NOUN" and writes these words along
 * with their series of numbers to an output file.
 *
 * @param inputFilename The name of the input file.
 * @param outputFilename The name of the output file where extracted nouns will be written.
 */
void extractNounsToFile(const std::string& inputFilename, const std::string& outputFilename) {

    std::ifstream inputFile(inputFilename);
    std::ofstream outputFile(outputFilename);

    if (!inputFile.is_open()) {
        std::cerr << "Unable to open input file: " << inputFilename << std::endl;
        return;
    }

    if (!outputFile.is_open()) {
        std::cerr << "Unable to open output file: " << outputFilename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        std::string word, value, numbers;
        std::getline(iss, word, '_'); // Extract the word part
        iss >> value; // Extract the VALUE part

        if (value == "NOUN") {
            if (std::getline(iss, numbers)) { // Extract the series of numbers
                outputFile << word << " " << numbers << std::endl;
            }
        }
    }
}

/**
 * @brief Realization of the extractNounsToFile function. Writes "Extraction complete" when done.
 */
void extractionOfNouns() {
    std::string inputFilename = "C:/dev (windows)/note-app/utils/188/2000.txt"; // Replace with your input file name
    std::string outputFilename = "C:/dev (windows)/note-app/utils/word_vectors.txt"; // Replace with your desired output file name

    extractNounsToFile(inputFilename, outputFilename);

    std::cout << "Extraction complete." << std::endl;
}

/**
 * @brief Retrieves the content of a specified file as a string.
 *
 * This function opens a file with the given filename and reads its contents
 * into a string. In case the file cannot be opened, an error message is displayed.
 *
 * @param filename The name (and path, if necessary) of the file to be read.
 * @return std::string A string containing the contents of the file.
 */
std::string openFileAndGetContent(std::string filename) {
    std::ifstream file(filename);
    std::string content;

    if (file) {
        std::stringstream buffer;
        buffer << file.rdbuf();
        content = buffer.str();
    } else {
        // Handle the error if the file can't be opened
        std::cerr<<"Error: Unable to open file '"<< filename <<"'"<<std::endl;
        return "";
    }

    return content;
}

/**
 * @brief Retrieves the contents of all files in a specified directory.
 *
 * This function iterates through each file in the given directory and reads
 * its content. It assumes that each entry in the directory is a regular file
 * and uses the openFileAndGetContent function to read the content of each file.
 * The content of each file is then stored and returned as a vector of strings.
 *
 * @note: This function does not recursively read files in subdirectories. It
 * only reads files that are directly in the specified directory. Additionally,
 * the function does not filter file types, so it reads all files regardless
 * of their format or extension.
 *
 * @param directoryPath The path to the directory whose file contents are to be read.
 * @return std::map<std::string, std::string> A map containing as key the filename and as value
 *         the contents of each file in the directory.
 */
std::map<std::string, std::string> getDirectoryFileContents(const std::string& directoryPath) {
    std::map<std::string, std::string> fileContentsMap;
    for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
        if (entry.is_regular_file()) {
            std::string filePath = entry.path().string();
            std::string fileName = entry.path().filename().string();
            fileContentsMap[fileName] = openFileAndGetContent(filePath);
        }
    }
    return fileContentsMap;
}

#endif //NOTE_APP_FILES_H
