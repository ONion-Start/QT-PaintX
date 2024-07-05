/********************************************************************************
** Form generated from reading UI file 'paintarea.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PAINTAREA_H
#define UI_PAINTAREA_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ScribbleArea
{
public:

    void setupUi(QWidget *ScribbleArea)
    {
        if (ScribbleArea->objectName().isEmpty())
            ScribbleArea->setObjectName("ScribbleArea");
        ScribbleArea->resize(400, 300);

        retranslateUi(ScribbleArea);

        QMetaObject::connectSlotsByName(ScribbleArea);
    } // setupUi

    void retranslateUi(QWidget *ScribbleArea)
    {
        ScribbleArea->setWindowTitle(QCoreApplication::translate("ScribbleArea", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ScribbleArea: public Ui_ScribbleArea {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PAINTAREA_H
