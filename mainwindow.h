#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QTextBrowser>
#include <QPushButton>
#include <QString>
#include <QFile>
#include <QDebug>
#include <QProcess>
#include <QDesktopServices>
#include <QFileInfo>
#include <QDir>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_submit_button_clicked();

    void on_submit_button_2_clicked();

    void on_submit_button_3_clicked();

    void on_show_output_button_clicked();

    void on_get_set_button_clicked();

    void on_run_all_button_clicked();

    void on_load_sample_button_clicked();

    void on_load_sample_button_2_clicked();

    void on_load_sample_button_3_clicked();

    void on_load_sample_button_4_clicked();

    void on_load_sample_button_5_clicked();

    void on_load_sample_button_6_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_testButton_clicked();

private:
    Ui::MainWindow *ui;
    QPlainTextEdit* input_editor;
    QTextBrowser* output_browser;
    QTextBrowser* first_set_browser;
    QTextBrowser* follow_set_browser;
    int local_type = -1;

    int parse_ll1(int type);
    int parse_lr0(int type);
    int parse_slr1(int type);
    int parse_lr1(int type);
    int parse_lalr1(int type);
    int generate_best_fit();
    void output();

};
#endif // MAINWINDOW_H
