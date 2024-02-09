//
// Created by alexa on 23/01/2024.
//

#ifndef NOTE_APP_CONVERSION_H
#define NOTE_APP_CONVERSION_H

#include <string>
#include <QString>


// Returns true if the string is alphanumeric
bool isAlphanumeric(const std::string& str);
QString removeFromQstring(QString originalString, std::string searchString);

#endif //NOTE_APP_CONVERSION_H
