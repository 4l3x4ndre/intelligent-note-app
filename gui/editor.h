//
// Created by alexa on 22/01/2024.
//

#ifndef NOTE_APP_EDITOR_H
#define NOTE_APP_EDITOR_H

#include <QMainWindow>
#include <QTextEdit>
#include <QKeyEvent>
#include "file_manager.h"


class Editor;

class KeyPressFilter : public QObject {
Q_OBJECT
public:
    bool eventFilter(QObject* watched, QEvent* event) final;
    void setWatcher(Editor* e){listener = e;}
private:
    Editor* listener;
};

class Editor : public QMainWindow{
Q_OBJECT

public:
    explicit Editor(QWidget *parent = nullptr);

    void onKeyPressed(QKeyEvent* key);

private:
    // ------------- Attributes ------------
    // UI
    QTextEdit* textEdit;
    QVBoxLayout* mainLayout;
    QHBoxLayout* filesEditorLayout;
    QHBoxLayout* topBar;
    QVBoxLayout* linkedFilesLayout;
    std::set<QPushButton*> linkedFiles_ui;

    QPushButton* openButton;
    QPushButton* saveButton;
    QPushButton* fontButton;
    QPushButton* boldButton;
    QPushButton* analyzeButton;

    // Behaviour
    bool isBold;
    std::string currentFile = "Cours 0";
    std::set<std::string> linkedFiles;
    std::map<std::string, std::vector<float>> wordVectors;

    // Using a buffer to detect actionnables ("title1", ...)
    // The buffer is cleared when space bar is pressed.
    std::string buffer;
    std::vector<std::string> actions = {"TITRE1", "TITRE2"};

    // Entities
    KeyPressFilter* keyPressFilter;
    FileManager* fileManager;

    // ------------------- Methods ----------
    void checkBuffer();
    void convertToHtmlTitle(int level);
    void updateLinkedFiles();

protected :
    // Override keyPressEvent to handle key events
    void keyPressEvent(QKeyEvent *event) override;
private slots:
    void newDocument();
    void open();
    void save();
    void selectFont();
    void boldTrigger();
    void italicTrigger();
    void analyzeTrigger();
};


#endif //NOTE_APP_EDITOR_H
