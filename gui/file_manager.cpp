//
// Created by alexa on 22/01/2024.
//

#include "file_manager.h"

FileManager::FileManager(QWidget* parent):QWidget(parent) {
    filelist_ui = new QVBoxLayout(this);
    filelist = new std::vector<std::string>();

    filelist->push_back("1");
    filelist->push_back("2");
    filelist->push_back("3");

    for (int i = 0; i < filelist->size(); i++) {
        files_ui.push_back(new QPushButton(this));
        files_ui[i]->setText(QString::fromStdString(filelist->operator[](i) ));
        filelist_ui->addWidget(files_ui[i]);
        files_ui[i]->show();
    }
}