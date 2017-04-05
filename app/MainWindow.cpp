#include "MainWindow.h"

#include <QMessageBox>
#include <ui_MainWindow.h>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->btnFindRoots, SIGNAL(clicked(bool)), this, SLOT(OnButtonFindRootsClicked()));
    connect(ui->btnAbout, SIGNAL(triggered(bool)), this, SLOT(OnButtonAboutClicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnButtonFindRootsClicked()
{
    try {
        QString expression = ui->txtExpression->text();
        if (expression.trimmed().isEmpty()) {
            throw std::runtime_error("Введите выражение");
        }

        int method = ui->cmbMethod->currentIndex();

        bool ok;

        double minX = ui->txtMinX->text().toDouble(&ok);
        if (!ok) throw std::runtime_error("Ошибка в значении левого предела");

        double maxX = ui->txtMaxX->text().toDouble(&ok);
        if (!ok) throw std::runtime_error("Ошибка в значении правого предела");

        double epsilon = ui->txtEpsilon->text().toDouble(&ok);
        if (!ok) throw std::runtime_error("Ошибка в значении точности");

        Function function = m_parser.CreateFunction(expression.toUtf8().constData());

        std::vector<double> roots;

        double step = 0.01;
		for (double x = minX; x < maxX; x += step) {
            if (function(x) == 0) roots.push_back(x);
			else if (function(x) * function(x + step) < 0) {
                double root;

                switch (method) {
                case 0:
                    root = function.FindRootIterations(x, x + step, epsilon);
                    break;
                case 1:
                    root = function.FindRootDihotomy(x, x + step, epsilon);
                    break;
                case 2:
                    root = function.FindRootNewton(x, x + step, epsilon);
                    break;
                default:
                    break;
                }

                roots.push_back(root);
            }
        }

        QString log = expression;
        expression += " = 0\nX: [" + QString::number(minX) + ", " + QString::number(maxX) + "], ";
        expression += "Ɛ = " + QString::number(epsilon) + "\n";
        expression += "Метод: " + ui->cmbMethod->currentText() + "\n";
        expression += "Корни:\n";

        for (auto root : roots) {
            expression += "  " + QString::number(root) + "\n";
        }

        Log(expression);
    }
    catch(const std::exception& e) {
        QMessageBox mb;
        mb.setWindowTitle("Ошибка");
        mb.setText(QString::fromUtf8(e.what()));
        mb.exec();
    }
}

void MainWindow::OnButtonAboutClicked()
{
	QMessageBox::about(nullptr, "О программе", R"(Курсовая про программированию за 2 семестр первого курса по программированию.
											   Выполнил студент группы ИВБО-01-16 Калинин И.М.

											   Эта программа находит корни любого уравнения, приведённого к виду f(x)=0 в заданном диапазоне с указанной точностью и методом.
											   Допустимые операторы: +, -, *, / или \, %, ^
											   Допустимые функции: abs, sqrt, cbrt, exp, ln, lg, log(a,b), max(a,b), sin, cos, tg, ctg, arcsin, arccos, arctg, arcctg
											   Допустимые константы: e, pi)");
}

void MainWindow::Log(const QString &text)
{
    ui->txtHistory->append("----------------\n" + text + "\n");
}
