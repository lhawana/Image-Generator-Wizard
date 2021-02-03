﻿#include "filechooser.h"

#include <QLabel>
#include <QImage>
#include <QlineEdit>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDropEvent>
#include <QMessageBox>
#include <QPainter>

#include "imagewizard.h"

FileChooser::FileChooser(const QString& title, ImageInfo* image, QWidget* parent) : WizardPage(parent), font("Calibri", 14) {
	QObject::connect(&chooser, &QFileDialog::fileSelected, this, &FileChooser::setFilePath);

	ui.setupUi(this);

	// this makes it so we can only choose a single file that exists; no directories or multiple files
	chooser.setFileMode(QFileDialog::FileMode::ExistingFile);

	// only allow us to choose files with the following extensions

	acceptedFileTypes = {
		"png",
	};

	std::string fileFilter = "Images (";
	for(int i = 0; i < acceptedFileTypes.size(); i++) {
		fileFilter += "*." + acceptedFileTypes[i];

		if(i < acceptedFileTypes.size() - 1)
			fileFilter += " ";
	}
	fileFilter += ")";

	chooser.setNameFilter(fileFilter.c_str());

	chosenFileName = findChild<QLineEdit*>("chosenFileName");
	imgLabel = findChild<QLabel*>("imgLabel");
	QLabel* titleLabel = findChild<QLabel*>("title");
	titleLabel->setText(title);
	titleLabel->setFont(font);

	selectedImage = image;

	setupView();
}

FileChooser::~FileChooser() {
}

bool FileChooser::isReady() {
	return selectedImage->loaded;
}

void FileChooser::chooseFile() {
	chooser.show();
}

void FileChooser::setFilePath(QString path) {
	chosenFileName->setText(path);

	loadImage(path);
}

void FileChooser::dragEnterEvent(QDragEnterEvent* event) {
	event->acceptProposedAction();
}

void FileChooser::dropEvent(QDropEvent* event) {
	QString url = event->mimeData()->urls().first().toLocalFile();
	if(url.isEmpty())
		return;

	bool validFileType = false;

	for(std::string fileType : acceptedFileTypes) {
		if(url.toUtf8().toLower().endsWith(fileType))
			validFileType = true;
	}

	if(validFileType) {
		setFilePath(url);
	}
	else {
		QMessageBox messageBox;
		messageBox.warning(0, "Error", "Invalid file type");
	}
}

void FileChooser::setupView() {
	this->setAcceptDrops(true);
}

void FileChooser::loadImage(QString& path) {
	selectedImage->image->load(path);
	*(selectedImage->path) = path;
	selectedImage->loaded = true;

	scaleImage(imgLabel->size());

	getWizard()->enableNext();
}

void FileChooser::scaleImage(const QSize& size) {
	if(!selectedImage->loaded)
		return;

	QPixmap p = QPixmap::fromImage(*(selectedImage->image));
	imgLabel->setPixmap(p.scaled(size.width(), size.height(), Qt::KeepAspectRatio));
}

void FileChooser::resizeEvent(QResizeEvent* e) {
	QWidget::resizeEvent(e);
	scaleImage(imgLabel->size());
}

void FileChooser::paintEvent(QPaintEvent* e) {
	if(selectedImage->loaded)
		return;

	int offset = 75; // how many pixels the rectangle is from the edges
	int radius = 15; // how many pixels the rectangle's corners are curved
	int textWidth = 150;
	int textHeight = 50;

	QPainter painter(this);
	QPen pen(Qt::DashLine);
	QTextOption options;
	options.setAlignment(Qt::AlignCenter);

	pen.setWidth(3);
	pen.setBrush(Qt::gray);
	pen.setCapStyle(Qt::RoundCap);
	painter.setPen(pen);
	painter.setFont(font);

	painter.setRenderHint(QPainter::Antialiasing);
	painter.drawRoundedRect(QRect(offset, offset, width() - 2 * offset, height() - 2 * offset), radius, radius);
	painter.drawText(
		QRect((width() - textWidth) / 2, (height() - textHeight) / 2, textWidth, textHeight),
		"Drag a .png file",
		options
	);
}

void FileChooser::reset() {
	WizardPage::reset();

	chosenFileName->clear();
	imgLabel->clear();
	selectedImage->reset();
}