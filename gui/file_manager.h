//
// Created by alexa on 22/01/2024.
//

#ifndef NOTE_APP_FILE_MANAGER_H
#define NOTE_APP_FILE_MANAGER_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <set>

class FileManager : public QWidget {
    Q_OBJECT
private:
    QVBoxLayout* filelist_ui;
    std::vector<QPushButton*> files_ui;
    std::vector<std::string>* filelist;

    std::map<std::string, std::set<std::string>> fileGraph;

public:

    explicit FileManager(QWidget* parent = nullptr);

    bool checkFilename(const std::string& filename) {
        return std::find(filelist->begin(), filelist->end(),filename) != filelist->end();
    }

    void addToFileGraph(std::string src, std::string dst) {
        fileGraph[src].insert(dst);
    }

    std::set<std::string> getFilesLinkedFrom(std::string src) {
        if (fileGraph.contains(src)) return fileGraph[src];
        return std::set<std::string>();
    }

};

#endif //NOTE_APP_FILE_MANAGER_H
