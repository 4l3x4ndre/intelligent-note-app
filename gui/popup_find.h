//
// Created by alexa on 23/01/2024.
//

#ifndef NOTE_APP_POPUP_FIND_H
#define NOTE_APP_POPUP_FIND_H

#include <QWidget>
#include <QInputDialog>

class Finder : public QWidget {
    Q_OBJECT
public:
    Finder(QWidget *parent = nullptr): QWidget(parent) {
        bool ok;
        QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                             tr("User name:"), QLineEdit::Normal,
                                             "file...", &ok);
    }

};


#endif //NOTE_APP_POPUP_FIND_H
