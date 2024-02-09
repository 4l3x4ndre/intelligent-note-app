//
// Created by alexa on 22/01/2024.
//

#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QFontDialog>
#include <iostream>
#include <algorithm>
#include <list>
#include <QTextBlock>
#include <QInputDialog>
#include "editor.h"
#include "../utils/conversion.h"
#include "../utils/nlp.h"
#include "../utils/files.h"

Editor::Editor(QWidget *parent): QMainWindow(parent) {
    // ---------------------- UI--------------------
    mainLayout = new QVBoxLayout();
    topBar = new QHBoxLayout();
    filesEditorLayout = new QHBoxLayout();
    linkedFilesLayout = new QVBoxLayout();

    textEdit = new QTextEdit();
    fileManager = new FileManager();

    openButton = new QPushButton("Open");
    topBar->addWidget(openButton);
    saveButton = new QPushButton("Save");
    topBar->addWidget(saveButton);
    fontButton = new QPushButton("Font");
    topBar->addWidget(fontButton);
    analyzeButton = new QPushButton("Analyze");
    topBar->addWidget(analyzeButton);
//    boldButton = new QPushButton("Bold");
//    topBar->addWidget(boldButton);

    filesEditorLayout->addWidget(fileManager);
    filesEditorLayout->addWidget(textEdit);

    mainLayout->addLayout(topBar);
    mainLayout->addLayout(filesEditorLayout);

    connect(openButton, &QPushButton::clicked, this, &Editor::open);
    connect(saveButton, &QPushButton::clicked, this, &Editor::save);
    connect(fontButton, &QPushButton::clicked, this, &Editor::selectFont);
    connect(analyzeButton, &QPushButton::clicked, this, &Editor::analyzeTrigger);


    QWidget * window = new QWidget();
    window->setLayout(mainLayout);
    setCentralWidget(window);

    // ------------------- Behavior ----------------
    isBold = false;
    buffer = "";

    // Keys
    keyPressFilter = new KeyPressFilter;
    keyPressFilter->setWatcher(this);
    textEdit->installEventFilter(keyPressFilter);

    // Init with a blank line
    textEdit->setText("\n");
    QTextCursor cursor = textEdit->textCursor();
    cursor.movePosition(QTextCursor::Start);
    textEdit->setTextCursor(cursor);

    // Retrieve files linked to current file
    updateLinkedFiles();
}

void Editor::updateLinkedFiles() {

    linkedFiles = fileManager->getFilesLinkedFrom(currentFile);

    // Delete each widget in the layout
    for (std::set<QPushButton*>::iterator  it = linkedFiles_ui.begin();
        it != linkedFiles_ui.end(); it ++){
        linkedFilesLayout->removeWidget(*it);
        delete *it;
    }
    linkedFiles_ui.clear();

    // Add back the widgets
    for (auto filename : linkedFiles) {
        QPushButton * linkedFileButton = new QPushButton(QString::fromStdString(filename));
        linkedFilesLayout->addWidget(linkedFileButton);
        linkedFiles_ui.insert(linkedFileButton);
    }
    filesEditorLayout->addLayout(linkedFilesLayout);

}

void Editor::newDocument()
{
    currentFile.clear();
    textEdit->setText(QString());
}

void Editor::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file");
    QFile file(fileName);
    currentFile = fileName.toStdString();
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
        return;
    }
    setWindowTitle(fileName);
    QTextStream in(&file);
    QString text = in.readAll();
    textEdit->setText(text);
    file.close();
}

void Editor::save()
{
    QString fileName;
    // If we don't have a filename from before, get one.
    if (currentFile.empty()) {
        fileName = QFileDialog::getSaveFileName(this, "Save");
        currentFile = fileName.toStdString();
    } else {
        fileName = QString::fromStdString(currentFile);
    }
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }
    setWindowTitle(fileName);
    QTextStream out(&file);
    QString text = textEdit->toPlainText();
    out << text;
    file.close();
}

void Editor::selectFont()
{
    bool fontSelected;
    QFont font = QFontDialog::getFont(&fontSelected, this);
    if (fontSelected)
        textEdit->setFont(font);
}

void Editor::boldTrigger() {
    QTextCursor cursor = textEdit->textCursor();
    QTextCharFormat format = cursor.charFormat();

    // Check if the current selection is already bold
    if (format.fontWeight() == QFont::Bold) {
        // Remove bold style
        format.setFontWeight(QFont::Normal);
    } else {
        // Apply bold style
        format.setFontWeight(QFont::Bold);
    }

    // Apply the new format to the selected text
    cursor.mergeCharFormat(format);
    textEdit->mergeCurrentCharFormat(format);
}

void Editor::italicTrigger() {
    QTextCursor cursor = textEdit->textCursor();
    QTextCharFormat format = cursor.charFormat();

    // Check if the current selection is already italic
    if (format.fontItalic()) {
        // Remove italic style
        format.setFontItalic(false);
    } else {
        // Apply italic style
        format.setFontItalic(true);
    }

    // Apply the new format to the selected text
    cursor.mergeCharFormat(format);
    textEdit->mergeCurrentCharFormat(format);
}

void Editor::analyzeTrigger() {
    std::cout<<"Analyzing..."<<std::endl;

    if (wordVectors.empty()) {
        std::cout<<"Retrieving word vectors... ";
        // Read the word vectors from file if it is the first time
        wordVectors = readDataFromFile("../utils/word_vectors.txt");
        std::cout<<"Word vectors retrieved."<<std::endl;
    }

    if (wordVectors.find("technology") != wordVectors.end()) {
        std::cout<<"tech"<<std::endl;
    }

    QString dirName = QFileDialog::getExistingDirectory(
            this,
            "Open a directory",
            "C:/",
            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    std::map<std::string, std::string> files = getDirectoryFileContents(dirName.toStdString());
    std::vector<float> textVec_1;
    std::vector<float> textVec_2;

    // Outer loop
    for (auto it1 = files.begin(); it1 != files.end(); ++it1) {
        // Inner loop starts from the element after it1
        for (auto it2 = it1; it2 != files.end(); ++it2) { // it2 = std::next(it1)
            std::cout << "Combination: " << it1->first << ", " << it2->first << "" << std::endl;
            // You can access the pair's values using it1->second and it2->second if needed
            textVec_1 = textToVector((*it1).second, wordVectors);
            textVec_2 = textToVector((*it2).second, wordVectors);
            std::cout << "     " <<cosineSimilarity(textVec_1, textVec_2) << std::endl;

        }

    }

    // Similarity with "technology"
    textVec_1 = textToVector("technology", wordVectors);
    for (auto it1 = files.begin(); it1 != files.end(); ++it1) {
        textVec_2 = textToVector((*it1).second, wordVectors);
        std::cout <<(*it1).first<< " with technology: " <<cosineSimilarity(textVec_1, textVec_2) << std::endl;
        std::cout<<"Vector: ";
        for (auto f : textVec_2) {
            std::cout<<f<<", ";
        }
        std::cout<<std::endl;
    }

    // Similarity with "science"
    textVec_1 = textToVector("science", wordVectors);
    for (auto it1 = files.begin(); it1 != files.end(); ++it1) {
        textVec_2 = textToVector((*it1).second, wordVectors);
        std::cout <<(*it1).first<< " with science: " <<cosineSimilarity(textVec_1, textVec_2) << std::endl;
    }

    std::cout<<" Done."<<std::endl;
}

bool KeyPressFilter::eventFilter(QObject* watched, QEvent* event) {
    if(event->type()==QEvent::KeyPress) {
        QKeyEvent* key = static_cast<QKeyEvent*>(event);
        std::cout<<"Got :"<<QKeySequence(key->key()).toString().toStdString()<<std::endl;
        if (listener) listener->onKeyPressed(key);
    } else {
        return QObject::eventFilter(watched, event);
    }
    return false;
}


void Editor::keyPressEvent(QKeyEvent *event)  {
    // Check for Ctrl+B for bold
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_B) {
        boldTrigger();
    // Check for Ctrl+I for italic
    } else if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_I) {
        italicTrigger();
    } else if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_L) {
        bool ok;
        std::string text = QInputDialog::getText(this, tr("Link with file..."),
                                             tr("Filename:"), QLineEdit::Normal,
                                             "Enter a filename here...", &ok).toStdString();
        fileManager->addToFileGraph(currentFile, text);
        updateLinkedFiles();
    } else {
        // Let the base class handle other key events
        QWidget::keyPressEvent(event);
    }
}

void Editor::onKeyPressed(QKeyEvent* key) {
    std::cout<<"Int version is "<<key->key()<<std::endl;
    int keyInt = key->key();
    if (keyInt == Qt::Key_Space) {
        std::cout<<"Buffer is : "<<buffer<<std::endl;
        if (std::find(actions.begin(), actions.end(),buffer) != actions.end()) checkBuffer();
        buffer = "";
    } else if (keyInt>= 65 && keyInt <= 90 ||
        keyInt >= 48 && keyInt <= 57) {
        buffer = buffer + QKeySequence(keyInt).toString().toStdString();
    } else if (keyInt == Qt::Key_Backspace && buffer.size() > 0) {
        buffer = buffer.substr(0, buffer.size() - 1);
    }
}

void Editor::checkBuffer() {
    if (buffer == "TITRE1") {
        convertToHtmlTitle(1);
    } else if (buffer == "TITRE2") {
        convertToHtmlTitle(2);
    }
}

void Editor::convertToHtmlTitle(int level) {
    QTextCursor cursor = textEdit->textCursor();
    QTextBlock block = cursor.block();

    // Find the start and end positions of the current line
    int lineStart = block.position();
    int lineEnd = lineStart + block.length();

    // Extract the entire line
    cursor.setPosition(lineStart);
    cursor.setPosition(lineEnd, QTextCursor::KeepAnchor);
    QString lineText = cursor.selectedText();//.trimmed();  // Trim to remove leading/trailing spaces
    std::transform(buffer.begin(), buffer.end(), buffer.begin(), ::tolower);
    lineText = removeFromQstring(lineText, buffer);

    if (!lineText.isEmpty()) {
        QString htmlTitle = QString("<h%1>%2</h%1>").arg(level).arg(lineText);

        // Replace the entire line with the HTML-formatted title
        cursor.removeSelectedText();
        cursor.insertHtml(htmlTitle);
    }
}