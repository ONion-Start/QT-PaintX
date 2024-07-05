#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QMainWindow>

class PaintArea;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void open();
    void save();
    void penColor();
    void penWidth();
    void rectangle();
    void star();
    void ellipse();
    void line();

private:
    void createActions();
    void createMenus();
    bool saveFile(const QByteArray &fileFormat);

    PaintArea *paintArea;

    QMenu *saveAsMenu;
    QMenu *fileMenu;
    QMenu *optionMenu;
    QMenu *figureMenu;

    QAction *openAct;

    QList<QAction *> saveAsActs;
    QAction *exitAct;
    QAction *addRectangle;
    QAction *addLine;
    QAction *addStar;
    QAction *addEllipse;
    QAction *penColorAct;
    QAction *penWidthAct;
    QAction *clearScreenAct;
};

#endif
