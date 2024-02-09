//
// Created by alexa on 23/01/2024.
//

#include <algorithm>
#include "conversion.h"


bool isAlphanumeric(const std::string& str) {
    // Check if all characters in the string are alphanumeric
    return std::all_of(
            str.begin(),
            str.end(),
            [](unsigned char ch) {
                return std::isalnum(ch);
            });
}

QString removeFromQstring(QString originalString, std::string searchString) {
    int index = originalString.indexOf(QString::fromStdString(searchString));
    if (index != -1) {
        int lengthToRemove = searchString.length();
        originalString.remove(index, lengthToRemove);
    }
    return originalString;
}