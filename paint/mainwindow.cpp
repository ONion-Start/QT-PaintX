#include <QtWidgets>

#include "mainwindow.h"
#include "paintarea.h"

MainWindow::MainWindow()
{
    paintArea = new PaintArea;
    setCentralWidget(paintArea);

    createActions();
    createMenus();
    setWindowTitle(tr("PaintX"));
    resize(1280, 760);
}

void MainWindow::closeEvent(QCloseEvent *event)
{

}

void MainWindow::open()
{

    QString fileName = QFileDialog::getOpenFileName(this,
                               tr("Open File"), QDir::currentPath());
    if (!fileName.isEmpty())
        paintArea->openImage(fileName);
}

void MainWindow::save()
{

    QAction *action = qobject_cast<QAction *>(sender());

    QByteArray fileFormat = action->data().toByteArray();

    saveFile(fileFormat);
}

void MainWindow::rectangle()
{
    paintArea->addRectangle();
}

void MainWindow::star()
{
    paintArea->addStar();
}

void MainWindow::ellipse()
{
    paintArea->addEllipse();
}

void MainWindow::line()
{
    paintArea->addLine();
}

void MainWindow::penColor()
{
    QColor newColor = QColorDialog::getColor(paintArea->penColor());

    if (newColor.isValid())
        paintArea->setPenColor(newColor);
}

void MainWindow::penWidth()
{
    bool ok;
    int newWidth = QInputDialog::getInt(this, tr("Scribble"),
                                        tr("Толщина:"),
                                        paintArea->penWidth(),
                                        1, 50, 1, &ok);
    if (ok)
        paintArea->setPenWidth(newWidth);
}

void MainWindow::createActions()
{
    openAct = new QAction(tr("&Открыть"), this);

    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    foreach (QByteArray format, QImageWriter::supportedImageFormats()) {
        QString text = tr("%1...").arg(QString(format).toUpper());

        QAction *action = new QAction(text, this);

        action->setData(format);

        connect(action, SIGNAL(triggered()), this, SLOT(save()));

        saveAsActs.append(action);
    }
    exitAct = new QAction(tr("Выйти"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    addRectangle = new QAction(tr("&Прямоугольник"), this);
    connect(addRectangle, SIGNAL(triggered()), this, SLOT(rectangle()));

    addEllipse = new QAction(tr("&Овал"), this);
    connect(addEllipse, SIGNAL(triggered()), this, SLOT(ellipse()));

    addStar = new QAction(tr("&Звезда"), this);
    connect(addStar, SIGNAL(triggered()), this, SLOT(star()));

    addLine = new QAction(tr("&Линия"), this);
    connect(addLine, SIGNAL(triggered()), this, SLOT(line()));

    penColorAct = new QAction(tr("&Цвет"), this);
    connect(penColorAct, SIGNAL(triggered()), this, SLOT(penColor()));

    penWidthAct = new QAction(tr("&Толщина"), this);
    connect(penWidthAct, SIGNAL(triggered()), this, SLOT(penWidth()));

    clearScreenAct = new QAction(tr("&Очистить"), this);
    connect(clearScreenAct, SIGNAL(triggered()),
            paintArea, SLOT(clearImage()));
}

void MainWindow::createMenus()
{
    saveAsMenu = new QMenu(tr("&Сохранить как"), this);
    foreach (QAction *action, saveAsActs)
        saveAsMenu->addAction(action);
    fileMenu = new QMenu(tr("&Файл"), this);
    fileMenu->addAction(openAct);
    fileMenu->addMenu(saveAsMenu);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);
    optionMenu = new QMenu(tr("&Настройки"), this);
    optionMenu->addAction(penColorAct);
    optionMenu->addAction(penWidthAct);
    optionMenu->addSeparator();
    optionMenu->addAction(clearScreenAct);
    figureMenu = new QMenu(tr("&Фигуры"), this);
    figureMenu->addAction(addRectangle);
    figureMenu->addAction(addEllipse);
    figureMenu->addAction(addLine);
    figureMenu->addAction(addStar);
    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(optionMenu);
    menuBar()->addMenu(figureMenu);
}

bool MainWindow::saveFile(const QByteArray &fileFormat)
{
    QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                               initialPath,
                               tr("%1 Files (*.%2);;All Files (*)")
                               .arg(QString::fromLatin1(fileFormat.toUpper()))
                               .arg(QString::fromLatin1(fileFormat)));
    if (fileName.isEmpty()) {
        return false;
    } else {
        return paintArea->saveImage(fileName, fileFormat.constData());
    }
}
