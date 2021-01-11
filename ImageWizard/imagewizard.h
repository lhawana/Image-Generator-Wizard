#pragma once

#include <QtWidgets/QWidget>
#include "ui_imagewizard.h"
#include "filechooser.h"

class ImageWizard : public QWidget
{
    Q_OBJECT 

public:
    ImageWizard(QWidget *parent = Q_NULLPTR);
    void addFrame(FileChooser& frame);

public slots:
    void goNext();          
    void goPrev();

private:
    Ui::ImageWizardClass ui;
    QStackedWidget* frames;                 //Holds pages for the UI 
};
