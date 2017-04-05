#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "Parser.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void OnButtonFindRootsClicked();
    void OnButtonAboutClicked();
private:
	void Log(const QString& text);

    Ui::MainWindow *ui;
    Parser m_parser;
};

#endif // MAINWINDOW_H
