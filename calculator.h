#ifndef CALCULATOR_H
#define CALCULATOR_H

#include<QMainWindow>
#include<QTextEdit>

namespace Ui {
class calculator;
}

class calculator : public QMainWindow
{
    Q_OBJECT

public:
    explicit calculator(QWidget *parent = 0);
    ~calculator();

private slots:

    //清空所有数据，重置界面显示
    void clear();

    //清空所有内部数据，不改变界面显示
    void clear_equation();

    //删除
    void btn_del();
    void btn_equal();
    void btn_0();
    void btn_1();
    void btn_2();
    void btn_3();
    void btn_4();
    void btn_5();
    void btn_6();
    void btn_7();
    void btn_8();
    void btn_9();
    void btn_point();
    void btn_pls();
    void btn_sub();
    void btn_mul();
    void btn_div();
    void btn_left();
    void btn_right();

private:
    Ui::calculator *ui;

    //上文本框最大字数
    const int MAX_TOP_LENGTH = 22;
    //下文本框最大字数
    const int MAX_BOTTOM_LENGTH = 21;
    //double转QString有效数字位数
    const int PRECISION = 10;

    /*注：由于×和÷是双字节字符，在运算符类型判断的时候造成麻烦，所以分成两个*/
    QString equation_for_show;      //屏幕显示算式，显示×和÷
    QString equation;               //内部运算算式，显示*和/

    //监控键盘按键
    void keyPressEvent(QKeyEvent * e);
    //主运算函数
    double calculate(QString & equation);
    //清空界面上上文本框的文本，并不改变数据
    void clear_top() const;
    //在下文本框内追加文字，改变数据
    void addAndChange_num(QString c);
    //继承上一次的运算结果
    void inherit_last_result();

    //上一次运算的最后一个运算数
    double last_num;
    //上一次运算的最后一个运算符号
    QChar last_signal;
    //上一次运算结果
    double last_result;

    //检测是否需要覆盖0
    void check_erase_0();
    //检测是否需要覆盖运算符
    void check_erase_signal();
    //检测是否需要抹掉小数点
    void check_erase_point();
};

#endif // CALCULATOR_H
