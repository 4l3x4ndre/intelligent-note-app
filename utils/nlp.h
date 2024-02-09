//
// Created by alexa on 28/01/2024.
//

#ifndef NOTE_APP_NLP_H
#define NOTE_APP_NLP_H

#include <sstream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <iterator>
#include <map>
#include <iostream>

/**
 * @brief Tokenizes a string into words.
 *
 * This function takes a string and splits it into individual words based on whitespace.
 * It uses an input string stream to iterate over the words in the given string and then
 * collects them into a vector. The function is efficient for parsing sentences or any
 * text into separate words.
 *
 * @note The function splits the string on spaces and does not account for punctuation.
 *       Thus, punctuation will be included in the words.
 *
 * @param text The string to be tokenized.
 * @return std::vector<std::string> A vector containing the individual words from the input string.
 */
std::vector<std::string> tokenize(const std::string& text) {
    std::istringstream iss(text);
    return std::vector<std::string>{std::istream_iterator<std::string>{iss},
                                    std::istream_iterator<std::string>{}};
}

/**
 * @brief Converts text to a vector representation.
 *
 * This function transforms a given text into a vector by aggregating the vectors of individual words in the text.
 * It tokenizes the input text into words, looks up each word's vector in the provided word vector map, and then
 * averages these vectors to create a single vector representing the entire text.
 *
 * @note The function assumes that word vectors are of 300 dimensions. Words not found in the wordVectors map will
 *       be ignored in the averaging process.
 *
 * @param text The string of text to be converted.
 * @param wordVectors A map where keys are words and values are their corresponding vector representations.
 * @return std::vector<float> A vector representing the averaged word vectors of the input text.
 */
std::vector<float> textToVector(const std::string& text,
                                const std::map<std::string, std::vector<float>>& wordVectors) {
    std::vector<std::string> words = tokenize(text);
    std::vector<float> textVec(300, 0.0f); // Assuming 300 dimensions

    int count = 0;
    for (const std::string& word : words) {
        auto it = wordVectors.find(word);
        if (it != wordVectors.end()) {
            // ------- sum -----
            std::transform(textVec.begin(), textVec.end(), it->second.begin(), textVec.begin(), std::plus<float>());
            count++;
        }
    }
    if (count > 0) {
        std::transform(textVec.begin(), textVec.end(), textVec.begin(), [count](float val) { return val / count; });
    }
    return textVec;
}

/**
 * @brief Reads data from a file and stores it in a map.
 *
 * This function opens and reads a specified file line by line. Each line is expected to contain a word followed by
 * a series of floating-point numbers. The word serves as the key in the map, and the series of numbers are stored
 * as a vector of floats.
 *
 * @note The function assumes that each line in the file is formatted with a word followed by space-separated
 *       floating-point numbers. Lines not conforming to this format may result in undefined behavior.
 *
 * @param filename The name of the file to be read.
 * @return std::map<std::string, std::vector<float>> A map where each key is a word from the file and each value
 *         is a vector of floats representing the associated numbers.
 */
std::map<std::string, std::vector<float>> readDataFromFile(const std::string& filename) {
    std::map<std::string, std::vector<float>> wordNumbersMap;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return wordNumbersMap;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string word;
        float number;
        std::vector<float> numbers;

        iss >> word; // Read the word
        while (iss >> number) { // Read the series of numbers
            numbers.push_back(number);
        }

        wordNumbersMap[word] = numbers;
    }

    return wordNumbersMap;
}

/**
 * @brief Calculates the dot product of two vectors.
 *
 * This function computes the dot product of two vectors. The dot product is
 * the sum of the products of the corresponding entries of the two sequences
 * of numbers. Both vectors must be of the same size.
 *
 * @param vec1 The first vector.
 * @param vec2 The second vector.
 * @return double The dot product of the two vectors. If the vectors are of
 *         different sizes, returns 0.0.
 */
float dotProduct(const std::vector<float>& a, const std::vector<float>& b) {
    float sum = 0.0f;
    for (size_t i = 0; i < a.size(); ++i) {
        sum += a[i] * b[i];
    }
    return sum;
}

/**
 * @brief Calculates the norm (magnitude) of a vector.
 *
 * This function computes the Euclidean norm, or the "length," of a vector.
 * The norm is calculated as the square root of the sum of the squares of
 * the vector's components.
 *
 * @param vec The vector whose norm is to be calculated.
 * @return double The Euclidean norm of the vector.
 */
float vectorNorm(const std::vector<float>& vec) {
    float sum = 0.0f;
    for (float v : vec) {
        sum += v * v;
    }
    return std::sqrt(sum);
}

/**
 * @brief Calculates the cosine similarity between two vectors.
 *
 * This function computes the cosine similarity between two vectors. Cosine
 * similarity is a measure of similarity between two non-zero vectors of an
 * inner product space that measures the cosine of the angle between them.
 * The cosine of 0° is 1, and it is less than 1 for any other angle. It is
 * thus a judgment of orientation and not magnitude.
 *
 * @param vec1 The first vector.
 * @param vec2 The second vector.
 * @return double The cosine similarity between the two vectors. Returns 0.0
 *         if either of the vectors is zero or if they are of different sizes.
 */
float cosineSimilarity(const std::vector<float>& a, const std::vector<float>& b) {
    if (a.size() != b.size()) {
        throw std::invalid_argument("Vectors must be of the same size.");
    }

    // ------- dot product ----------
    float dot = dotProduct(a, b);
    float normA = vectorNorm(a);
    float normB = vectorNorm(b);

    if (normA == 0.f || normB == 0.f) {
        return 0.f; // To handle divide-by-zero
    }

    return dot / (normA * normB);
}

#endif //NOTE_APP_NLP_H
