#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdio.h>
#include <stdlib.h>
#include "LL1.h"
#include "LR0.h"
#include "SLR1.h"
#include "LR1.h"
#include "LALR1.h"
QString str2qstr(const string &s)
{
return QString(QString::fromLocal8Bit(s.c_str()));
}
string qstr2str(const QString &s)
{
return string((const char *)s.toLocal8Bit());
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("编译原理实验程序");
    input_editor = findChild<QPlainTextEdit*>("input_editor");
    output_browser = findChild<QTextBrowser*>("output_browser");
    first_set_browser = findChild<QTextBrowser*>("first_set_browser");
    follow_set_browser = findChild<QTextBrowser*>("follow_set_browser");
    QDir log_folder("./log");
    if (log_folder.exists()) {
        system("del /F /S /Q .\\log");
    }
    if (!log_folder.exists()) {
        log_folder.mkdir(QDir::currentPath() + "/log");
    }
}

MainWindow::~MainWindow()
{
    QDir log_folder("./log");
    if (log_folder.exists()) {
        system("del /F /S /Q .\\log");
    }
    delete ui;
}

void MainWindow::output()
{
    QString result_string;
    QFile file_out("log/temp_out.txt");
    if (file_out.open(QIODevice::ReadOnly)) {
        result_string = file_out.readAll();
    }
    file_out.close();

    output_browser->setText(result_string);
}

// 文法分析函数
int MainWindow::parse_ll1()
{
    QString input_string = input_editor->toPlainText();

    QFile file_in("log/temp_in.txt");
    if (file_in.open(QIODevice::WriteOnly)) {
        file_in.write(input_string.toLatin1().data());
    }
    file_in.close();

    int result_indicator = run_ll1("log/temp_in.txt", "log/temp_out.txt");
    cout << "LL1 - " << result_indicator << endl;

    return result_indicator;
}

int MainWindow::parse_lr0()
{
    QString input_string = input_editor->toPlainText();

    QFile file_in("log/temp_in.txt");
    if (file_in.open(QIODevice::WriteOnly)) {
        file_in.write(input_string.toLatin1().data());
    }
    file_in.close();

    int result_indicator = run_lr0("log/temp_in.txt", "log/temp_out.txt");
    cout << "LR0 - " << result_indicator << endl;

    return result_indicator;
}

int MainWindow::parse_slr1()
{
    QString input_string = input_editor->toPlainText();

    QFile file_in("log/temp_in.txt");
    if (file_in.open(QIODevice::WriteOnly)) {
        file_in.write(input_string.toLatin1().data());
    }
    file_in.close();

    int result_indicator = run_slr1("log/temp_in.txt", "log/temp_out.txt");
    cout << "SLR1 - " << result_indicator << endl;

    return result_indicator;
}

int MainWindow::parse_lr1()
{
    QString input_string = input_editor->toPlainText();

    QFile file_in("log/temp_in.txt");
    if (file_in.open(QIODevice::WriteOnly)) {
        file_in.write(input_string.toLatin1().data());
    }
    file_in.close();

    int result_indicator = run_lr1andslr1("log/temp_in.txt", "log/temp_out.txt");
    cout << "LR1 - " << result_indicator << endl;
    cout<<"test text----------------------------------------"<<endl;
    return result_indicator;
}

int MainWindow::parse_lalr1()
{
    QString input_string = input_editor->toPlainText();

    QFile file_in("log/temp_in.txt");
    if (file_in.open(QIODevice::WriteOnly)) {
        file_in.write(input_string.toLatin1().data());
    }
    file_in.close();

    int result_indicator = run_lalr1("log/temp_in.txt", "log/temp_out.txt");
    cout << "LALR1 - " << result_indicator << endl;
    cout<<"test text----------------------------------------"<<endl;
    return result_indicator;
}

// 生成判断文法类型
int MainWindow::generate_best_fit()
{
    //int failed_count = 0;
    //int file_size[4] = {0};
    //QString paths[5] = {"log/temp_table_ll1", "log/temp_table_lr0", "log/temp_table_slr1", "log/temp_table_lalr1","log/temp_table_lr1"};
    //int ll1=parse_ll1(),lr0=parse_lr0(),slr1=parse_slr1(),lr1=parse_lr1();

    //local_type init value: -1
    int ll1_judge = 0;
    if(parse_ll1()==0)
    {
        ll1_judge = 10;
    }
    if(parse_lr0()==0)
    {
        local_type = 1;
        return local_type+ll1_judge;
    }
    if(parse_slr1()==0)
    {
        local_type = 2;
        return local_type+ll1_judge;
    }
    if(parse_lalr1()==0)
    {
        local_type = 3;
        return local_type+ll1_judge;
    }
    if(parse_lr1()==0)
    {
        local_type = 4;
        return local_type+ll1_judge;
    }
    /*if (parse_ll1() == 0) {
        QFileInfo table_ll1(paths[0]);
        file_size[0] = table_ll1.size();
    } else {
        failed_count++;
    }

    if (parse_lr0() == 0) {
        QFileInfo table_lr0(paths[1]);
        file_size[1] = table_lr0.size();
    } else {
        failed_count++;
    }

    if (parse_slr1() == 0) {
        QFileInfo table_slr1(paths[2]);
        file_size[2] = table_slr1.size();
    } else {
        failed_count++;
    }

    if (parse_lr1() == 0){
        QFileInfo table_lr1(paths[3]);
        file_size[3] = table_lr1.size();
    } else {
        failed_count++;
    }

    if (failed_count != 4) {
        // choose right table
        int table_index = 0;
        int max_size = -1;
        for (int i = 0; i < 4; i++) {
            if (file_size[i] > max_size) {
                table_index = i;
                max_size = file_size[i];
            }
        }
        local_type = table_index;
    } else {
        // not in the 4 types
        local_type = failed_count;
    }*/
    return local_type+ll1_judge/10;
}

// 槽函数定义
void MainWindow::on_submit_button_clicked()
{
    parse_ll1();
    output();
}

void MainWindow::on_submit_button_2_clicked()
{
    parse_lr0();
    output();
}

void MainWindow::on_submit_button_3_clicked()
{
    parse_slr1();
    output();
}

void MainWindow::on_pushButton_3_clicked()
{
    parse_lr1();
    output();
}

void MainWindow::on_pushButton_4_clicked()
{
    parse_lalr1();
    output();

}
void MainWindow::on_show_output_button_clicked()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile("log/temp_out.txt"));
}

void MainWindow::on_get_set_button_clicked()
{
    QString input_string = input_editor->toPlainText();

    QFile file_in("log/temp_in.txt");
    if (file_in.open(QIODevice::WriteOnly)) {
        file_in.write(input_string.toLatin1().data());
    }
    file_in.close();

    generate_best_fit();
    cout << "get sets: local_type=" << local_type << endl;
    if (local_type == 0)
        get_first_set_ll1("log/temp_in.txt", "log/temp_out.txt");
    else
        get_first_set_slr1("log/temp_in.txt", "log/temp_out.txt");

    QFile file_out1("log/temp_out.txt");
    if (file_out1.open(QIODevice::ReadOnly)) {
        first_set_browser->setText(file_out1.readAll());
    }
    file_out1.close();

    if (local_type == 0)
        get_follow_set_ll1("log/temp_in.txt", "log/temp_out.txt");
    else
        get_follow_set_slr1("log/temp_in.txt", "log/temp_out.txt");

    QFile file_out2("log/temp_out.txt");
    if (file_out2.open(QIODevice::ReadOnly)) {
        follow_set_browser->setText(file_out2.readAll());
    }
    file_out2.close();
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void MainWindow::on_run_all_button_clicked()
{
    int table_index = generate_best_fit();
    local_type = -1;
    QString paths[5] = {"log/temp_table_ll1", "log/temp_table_lr0", "log/temp_table_slr1", "log/temp_table_lalr1", "log/temp_table_lr1"};

    switch(table_index) {

    case 1:     parse_lr0();                break;
    case 2:     parse_slr1();               break;
    case 3:     parse_lalr1();              break;
    case 4:     parse_lr1();                break;
    case 11:    parse_lr0();parse_ll1();    break;
    case 12:    parse_slr1();parse_ll1();   break;
    case 13:    parse_lalr1();parse_ll1();  break;
    case 14:    parse_lr1();parse_ll1();    break;
    case 0:     parse_ll1();    break;
    default:    ui->output_browser->setText("【不属于四类文法】");                       return;
    }

    // display the chosen table
    QString result_string;
    QFile file_out(paths[table_index%10]);
    if (file_out.open(QIODevice::ReadOnly)) {
        result_string = file_out.readAll();
    }
    file_out.close();

    if(table_index != 0)output_browser->setText(result_string);
    else output_browser->setText(QString("【不属于四类文法】\n但")+result_string);

    if(table_index>10)
    {
        QString extra_string;
        QFile file_out_extra(paths[0]);
        if (file_out_extra.open(QIODevice::ReadOnly)) {
            extra_string = file_out_extra.readAll();
        }
        file_out_extra.close();

        auto textCursor = ui->output_browser->textCursor();
        textCursor.movePosition(QTextCursor::End);
        ui->output_browser->setTextCursor(textCursor);
        ui->output_browser->insertPlainText(QString("\n亦")+extra_string);
    }
}


void MainWindow::on_load_sample_button_clicked()
{
    input_editor->setPlainText("10\nE->TA\nA->+TA\nA->-TA\nA->&\nT->FB\nB->*FB\nB->/FB\nB->&\nF->(E)\nF->n");
}

void MainWindow::on_load_sample_button_2_clicked()
{
    input_editor->setPlainText("5\nS->E\nE->aAcBe\nA->b\nA->Ab\nB->d");
}

void MainWindow::on_load_sample_button_3_clicked()
{
    input_editor->setPlainText("7\nS->E\nE->E+T\nE->T\nT->T*F\nT->F\nF->(E)\nF->i");
}

void MainWindow::on_load_sample_button_4_clicked()
{
    input_editor->setPlainText("8\nE->TE'\nE'->+TE'\nE'->~\nT->FT'\nT'->*FT'\nT'->~\nF->(E)\nF->i");
}

void MainWindow::on_load_sample_button_5_clicked()
{
    input_editor->setPlainText("3\nS'->S\nS->Sa\nS->a");
}

void MainWindow::on_load_sample_button_6_clicked()
{
    input_editor->setPlainText("5\nS->AaB\nA->aA\nA->a\nB->Bd\nB->b");
}


void MainWindow::on_pushButton_2_clicked()
{
    input_editor->setPlainText("4\nA'->A\nA->BB\nB->aB\nB->b");
}



void MainWindow::on_pushButton_clicked()
{
    input_editor->setPlainText("7\nS'->S\nS->Aa\nS->bAc\nS->Bc\nS->bBa\nA->d\nB->d\n");
}




