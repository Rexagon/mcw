#include "MainWindow.h"

#include <QTextStream>
#include <QMessageBox>
#include <QScrollBar>
#include <QFile>
#include <ui_MainWindow.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

	QFile file("./history.txt");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QTextStream textStream(&file);
		textStream.setCodec("UTF-8");
		ui->txtHistory->append(textStream.readAll());
	}

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

        bool ok;

        double minX = ui->txtMinX->text().toDouble(&ok);
        if (!ok) throw std::runtime_error("Ошибка в значении левого предела");

        double maxX = ui->txtMaxX->text().toDouble(&ok);
        if (!ok) throw std::runtime_error("Ошибка в значении правого предела");

        double epsilon = ui->txtEpsilon->text().toDouble(&ok);
        if (!ok) throw std::runtime_error("Ошибка в значении точности");

		int method = ui->cmbMethod->currentIndex();

        Function function = m_parser.CreateFunction(expression.toStdString());

		std::vector<Root> roots = function.FindRoots(minX, maxX, epsilon, method);

		QString log = expression;
        expression += " = 0\nX: [" + QString::number(minX) + ", " + QString::number(maxX) + "], ";
		expression += "Eps = " + QString::number(epsilon) + "\n";
        expression += "Метод: " + ui->cmbMethod->currentText() + "\n";
        expression += "Корни:\n";

		for (unsigned int i = 0; i < roots.size(); ++i) {
			expression += "  " + QString::number(roots[i].x) + "\t(" + QString::number(roots[i].numIterations) + " итер.)\n";
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
    QMessageBox::about(nullptr,
"О программе", R"(Курсовая про программированию за 2 семестр первого курса по программированию.
Выполнил студент группы ИВБО-01-16 Калинин И.М.

Эта программа находит корни любого уравнения, приведённого к виду f(x)=0 в заданном диапазоне с указанной точностью и методом.
Допустимые операторы: +, -, *, / или \, %, ^
Допустимые функции: abs, sqrt, cbrt, exp, ln, lg, log(a,b), max(a,b), sin, cos, tg, ctg, arcsin, arccos, arctg, arcctg
Допустимые константы: e, pi)");
}

void MainWindow::Log(const QString &text)
{
	QString record = "----------------\n" + text + "\n";
	ui->txtHistory->append(record);
	QFile file("history.txt");
	if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
		QTextStream textStream(&file);
		textStream.setCodec("UTF-8");
		textStream << record;
	}
}
