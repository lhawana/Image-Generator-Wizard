﻿#pragma once

#include <QtWidgets/QWidget>
#include <QMainWindow>
#include <QFileDialog>
#include <vector>
#include <string>

#include "ui_filechooser.h"
#include "imageinfo.h"

class FileChooser : public QWidget {
	Q_OBJECT

public:
	FileChooser(const QString& title, ImageInfo* image, QWidget* parent = Q_NULLPTR);
	~FileChooser();

public slots:
	void chooseFile();
	void setFilePath(QString url);

	void dragEnterEvent(QDragEnterEvent* event);
	void dropEvent(QDropEvent* event);
	void resizeEvent(QResizeEvent* e);

private:
	Ui::FileChooserClass ui;
	QFileDialog chooser;
	QLineEdit* chosenFileName;
	QLabel* imgLabel;
	ImageInfo* selectedImage;
	std::vector<std::string> acceptedFileTypes;

	void setupView();
	void loadImage(QString& path);
	void scaleImage(const QSize& size);
};