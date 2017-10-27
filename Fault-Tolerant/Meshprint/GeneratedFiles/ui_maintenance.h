/********************************************************************************
** Form generated from reading UI file 'maintenance.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINTENANCE_H
#define UI_MAINTENANCE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MyDialog
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *widget;
    QWidget *verticalLayoutWidget_4;
    QVBoxLayout *verticalLayout_4;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QCheckBox *checkBox;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_5;
    QCheckBox *checkBox_7;
    QLabel *label_5;
    QSpacerItem *horizontalSpacer_5;
    QHBoxLayout *horizontalLayout_4;
    QCheckBox *checkBox_6;
    QLabel *label_4;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout_3;
    QCheckBox *checkBox_5;
    QLabel *label_3;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *verticalSpacer_4;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QWidget *widget_2;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout_6;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer_3;
    QPushButton *pushButton_9;
    QPushButton *pushButton_6;
    QPushButton *pushButton_5;
    QVBoxLayout *verticalLayout_3;
    QSpacerItem *verticalSpacer_5;
    QPushButton *pushButton_7;
    QHBoxLayout *horizontalLayout_7;
    QPushButton *pushButton_4;
    QPushButton *pushButton_3;
    QHBoxLayout *horizontalLayout_8;
    QPushButton *pushButton_11;
    QPushButton *pushButton_10;
    QLabel *label;

    void setupUi(QDialog *MyDialog)
    {
        if (MyDialog->objectName().isEmpty())
            MyDialog->setObjectName(QStringLiteral("MyDialog"));
        MyDialog->resize(440, 445);
        MyDialog->setMinimumSize(QSize(440, 445));
        MyDialog->setMaximumSize(QSize(440, 445));
        gridLayout = new QGridLayout(MyDialog);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        tabWidget = new QTabWidget(MyDialog);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        widget = new QWidget();
        widget->setObjectName(QStringLiteral("widget"));
        verticalLayoutWidget_4 = new QWidget(widget);
        verticalLayoutWidget_4->setObjectName(QStringLiteral("verticalLayoutWidget_4"));
        verticalLayoutWidget_4->setGeometry(QRect(0, 0, 391, 331));
        verticalLayout_4 = new QVBoxLayout(verticalLayoutWidget_4);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(10, 10, 0, 0);
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(-1, 10, -1, -1);
        checkBox = new QCheckBox(verticalLayoutWidget_4);
        checkBox->setObjectName(QStringLiteral("checkBox"));
        checkBox->setMaximumSize(QSize(191, 16777215));

        horizontalLayout_2->addWidget(checkBox);

        label_2 = new QLabel(verticalLayoutWidget_4);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_2->addWidget(label_2);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout_4->addLayout(horizontalLayout_2);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(-1, 10, -1, -1);
        checkBox_7 = new QCheckBox(verticalLayoutWidget_4);
        checkBox_7->setObjectName(QStringLiteral("checkBox_7"));
        checkBox_7->setMaximumSize(QSize(191, 16777215));

        horizontalLayout_5->addWidget(checkBox_7);

        label_5 = new QLabel(verticalLayoutWidget_4);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout_5->addWidget(label_5);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_5);


        verticalLayout_4->addLayout(horizontalLayout_5);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(-1, 10, -1, -1);
        checkBox_6 = new QCheckBox(verticalLayoutWidget_4);
        checkBox_6->setObjectName(QStringLiteral("checkBox_6"));
        checkBox_6->setMaximumSize(QSize(191, 16777215));

        horizontalLayout_4->addWidget(checkBox_6);

        label_4 = new QLabel(verticalLayoutWidget_4);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_4->addWidget(label_4);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_4);


        verticalLayout_4->addLayout(horizontalLayout_4);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(-1, 10, -1, -1);
        checkBox_5 = new QCheckBox(verticalLayoutWidget_4);
        checkBox_5->setObjectName(QStringLiteral("checkBox_5"));
        checkBox_5->setMaximumSize(QSize(191, 16777215));

        horizontalLayout_3->addWidget(checkBox_5);

        label_3 = new QLabel(verticalLayoutWidget_4);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_3->addWidget(label_3);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);


        verticalLayout_4->addLayout(horizontalLayout_3);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_4);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, 10, -1, -1);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton = new QPushButton(verticalLayoutWidget_4);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout->addWidget(pushButton);

        pushButton_2 = new QPushButton(verticalLayoutWidget_4);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        horizontalLayout->addWidget(pushButton_2);


        verticalLayout_4->addLayout(horizontalLayout);

        tabWidget->addTab(widget, QString());
        widget_2 = new QWidget();
        widget_2->setObjectName(QStringLiteral("widget_2"));
        horizontalLayoutWidget = new QWidget(widget_2);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(-1, -1, 401, 271));
        horizontalLayout_6 = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, -1, -1, -1);
        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_3);

        pushButton_9 = new QPushButton(horizontalLayoutWidget);
        pushButton_9->setObjectName(QStringLiteral("pushButton_9"));

        verticalLayout_2->addWidget(pushButton_9);

        pushButton_6 = new QPushButton(horizontalLayoutWidget);
        pushButton_6->setObjectName(QStringLiteral("pushButton_6"));

        verticalLayout_2->addWidget(pushButton_6);

        pushButton_5 = new QPushButton(horizontalLayoutWidget);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));

        verticalLayout_2->addWidget(pushButton_5);


        horizontalLayout_6->addLayout(verticalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, -1, -1, -1);
        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_5);

        pushButton_7 = new QPushButton(horizontalLayoutWidget);
        pushButton_7->setObjectName(QStringLiteral("pushButton_7"));

        verticalLayout_3->addWidget(pushButton_7);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        pushButton_4 = new QPushButton(horizontalLayoutWidget);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));

        horizontalLayout_7->addWidget(pushButton_4);

        pushButton_3 = new QPushButton(horizontalLayoutWidget);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));

        horizontalLayout_7->addWidget(pushButton_3);


        verticalLayout_3->addLayout(horizontalLayout_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        pushButton_11 = new QPushButton(horizontalLayoutWidget);
        pushButton_11->setObjectName(QStringLiteral("pushButton_11"));

        horizontalLayout_8->addWidget(pushButton_11);

        pushButton_10 = new QPushButton(horizontalLayoutWidget);
        pushButton_10->setObjectName(QStringLiteral("pushButton_10"));

        horizontalLayout_8->addWidget(pushButton_10);


        verticalLayout_3->addLayout(horizontalLayout_8);


        horizontalLayout_6->addLayout(verticalLayout_3);

        tabWidget->addTab(widget_2, QString());

        verticalLayout->addWidget(tabWidget);

        label = new QLabel(MyDialog);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout->addWidget(label);


        gridLayout->addLayout(verticalLayout, 1, 0, 1, 1);


        retranslateUi(MyDialog);
        QObject::connect(pushButton_10, SIGNAL(clicked()), MyDialog, SLOT(reject()));
        QObject::connect(pushButton, SIGNAL(clicked()), MyDialog, SLOT(reject()));
        QObject::connect(MyDialog, SIGNAL(rejected()), MyDialog, SLOT(DialogClose()));
        QObject::connect(pushButton_2, SIGNAL(clicked()), MyDialog, SLOT(on_updateButton_clicked()));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MyDialog);
    } // setupUi

    void retranslateUi(QDialog *MyDialog)
    {
        MyDialog->setWindowTitle(QApplication::translate("MyDialog", "Dialog", Q_NULLPTR));
        checkBox->setText(QString());
        label_2->setText(QApplication::translate("MyDialog", "\344\272\244\345\217\211\344\270\211\350\247\222\351\235\242\347\211\207", Q_NULLPTR));
        checkBox_7->setText(QString());
        label_5->setText(QApplication::translate("MyDialog", "\351\207\215\345\217\240\344\270\211\350\247\222\351\235\242\347\211\207", Q_NULLPTR));
        checkBox_6->setText(QString());
        label_4->setText(QApplication::translate("MyDialog", "\345\255\224\346\264\236", Q_NULLPTR));
        checkBox_5->setText(QString());
        label_3->setText(QApplication::translate("MyDialog", "\345\243\263\344\275\223", Q_NULLPTR));
        pushButton->setText(QApplication::translate("MyDialog", "\345\205\263\351\227\255", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("MyDialog", "\346\233\264\346\226\260", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(widget), QApplication::translate("MyDialog", "\350\257\212\346\226\255", Q_NULLPTR));
        pushButton_9->setText(QApplication::translate("MyDialog", "\350\207\252\345\212\250\344\277\256\345\244\215", Q_NULLPTR));
        pushButton_6->setText(QApplication::translate("MyDialog", "\346\240\207\350\256\260\347\233\270\344\272\244", Q_NULLPTR));
        pushButton_5->setText(QApplication::translate("MyDialog", "\345\210\233\345\273\272\344\270\211\350\247\222\351\235\242\347\211\207", Q_NULLPTR));
        pushButton_7->setText(QApplication::translate("MyDialog", "\345\244\207\347\224\250", Q_NULLPTR));
        pushButton_4->setText(QApplication::translate("MyDialog", "color", Q_NULLPTR));
        pushButton_3->setText(QApplication::translate("MyDialog", "\351\207\215\347\275\256", Q_NULLPTR));
        pushButton_11->setText(QApplication::translate("MyDialog", "\345\270\256\345\212\251", Q_NULLPTR));
        pushButton_10->setText(QApplication::translate("MyDialog", "\345\205\263\351\227\255", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(widget_2), QApplication::translate("MyDialog", "\344\277\256\345\244\215", Q_NULLPTR));
        label->setText(QApplication::translate("MyDialog", "\344\270\272\344\272\206\351\201\277\345\205\215\345\210\206\346\236\220\344\270\215\345\277\205\350\246\201\347\232\204\351\241\271\347\233\256\357\274\214\350\257\267\351\200\211\346\213\251\351\234\200\350\246\201\345\210\206\346\236\220\347\232\204\347\261\273\345\210\253\357\274\214\347\204\266\345\220\216\347\202\271\345\207\273\346\233\264\346\226\260\346\214\211\351\222\256\343\200\202", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MyDialog: public Ui_MyDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINTENANCE_H
