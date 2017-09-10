#include "calculator.h"
#include "ui_calculator.h"
#include<QKeyEvent>
#include<QDebug>
#include<QString>
#include<QVector>

static bool is_signal(QChar c);      //判断c是不是运算符

calculator::calculator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::calculator)
{
    ui->setupUi(this);

    /*初始化数据*/
    equation = equation_for_show = "0";
    ui->bottom_output->setText(equation_for_show);
    ui->top_output->setMaxLength(MAX_TOP_LENGTH);
    ui->bottom_output->setMaxLength(MAX_BOTTOM_LENGTH);
    last_num = 0;
    last_signal = '+';
    last_result = 0;

    /*QSS设置*/
    setStyleSheet(
                "QMainWindow{background-color: rgb(43,42,42);}"
                "QPushButton{border:1px hidden;border-radius:10px;padding:2px 4px;color:rgb(200,200,200);font-size:25px;}"
                "QPushButton:hover{background-color: rgb(136,134,132);}"
                "QPushButton:pressed{border:1px inset rgb(106,104,102);font-size: 23px}"
                "#output_frame{background-color: rgb(106,104,102);border:1px solid gray;border-radius:20px;padding: 2px 10px;}"
                "#top_output,#bottom_output{background: transparent;}"
                "#top_output{color:rgb(190,190,190);}"
                );

    /*槽关联*/
    connect(ui->btn_clear,&QPushButton::clicked,this,clear);
    connect(ui->btn_del,&QPushButton::clicked,this,btn_del);
    connect(ui->btn_0,&QPushButton::clicked,this,btn_0);
    connect(ui->btn_1,&QPushButton::clicked,this,btn_1);
    connect(ui->btn_2,&QPushButton::clicked,this,btn_2);
    connect(ui->btn_3,&QPushButton::clicked,this,btn_3);
    connect(ui->btn_4,&QPushButton::clicked,this,btn_4);
    connect(ui->btn_5,&QPushButton::clicked,this,btn_5);
    connect(ui->btn_6,&QPushButton::clicked,this,btn_6);
    connect(ui->btn_7,&QPushButton::clicked,this,btn_7);
    connect(ui->btn_8,&QPushButton::clicked,this,btn_8);
    connect(ui->btn_9,&QPushButton::clicked,this,btn_9);
    connect(ui->btn_pls,&QPushButton::clicked,this,btn_pls);
    connect(ui->btn_sub,&QPushButton::clicked,this,btn_sub);
    connect(ui->btn_mul,&QPushButton::clicked,this,btn_mul);
    connect(ui->btn_div,&QPushButton::clicked,this,btn_div);
    connect(ui->btn_point,&QPushButton::clicked,this,btn_point);
    connect(ui->btn_left,&QPushButton::clicked,this,btn_left);
    connect(ui->btn_right,&QPushButton::clicked,this,btn_right);
    connect(ui->btn_equal,&QPushButton::clicked,this,btn_equal);
}

calculator::~calculator()
{
    delete ui;
}
//初始顶层显示为空
void calculator::clear_top() const
{
    ui->top_output->setText("");
}
//更新显示框,在下文本框内追加文字，改变数据
void calculator::addAndChange_num(QString c)
{
    equation_for_show += c;
    equation += c;
    ui->bottom_output->setText(equation_for_show);
}
//继承上一次的运算结果
void calculator::inherit_last_result()
{
    if(equation == "0")
    {
        equation = QString::number(last_result,'g',PRECISION);
        equation_for_show = QString::number(last_result,'g',PRECISION);
    }
}
//检测是否需要覆盖0
void calculator::check_erase_0()
{
    if(equation == "0" || (equation[equation.size()-1] == '0'&&is_signal(equation[equation.size()-2])))//内部运算算式是否为0或只一个数字或空
    {
        equation_for_show.resize(equation.size()-1);
        equation.resize(equation.size()-1);
    }
}
//检测是否需要覆盖运算符
void calculator::check_erase_signal()
{
    if(is_signal(equation[equation.size()-1]))
    {
        equation_for_show.resize(equation.size()-1);
        equation.resize(equation.size()-1);
    }
}
//检测是否需要抹掉小数点
void calculator::check_erase_point()
{
    if(equation[equation.size()-1] == '.')
        btn_del();
}
//清空所有数据，重置界面显示
void calculator::clear()
{
    clear_equation();//清空所有内部数据，不改变界面显示
    ui->bottom_output->setText(equation_for_show);
    clear_top();//清空界面上上文本框的文本，并不改变数据
    /*重置*/
    last_num = 0;
    last_signal = '+';
    last_result = 0;
}
//清空所有内部数据，不改变界面显示
void calculator::clear_equation()
{
    equation = "0";
    equation_for_show = "0";
}
//删除
void calculator::btn_del()
{
    if(equation.size()>1)
    {
        equation_for_show.resize(equation.size()-1);
        equation.resize(equation.size()-1);
        ui->bottom_output->setText(equation_for_show);
    }
    else if(equation.size()==1)
    {
        equation=equation_for_show= "0";
        ui->bottom_output->setText(equation_for_show);
    }
}

/* 数字按键操作 0~9 */
void calculator::btn_0()
{
    if(equation.size() == MAX_BOTTOM_LENGTH)//超限
        return;
    for(int i=equation.size()-1;i>=0;i--)
    {
        if(i == 0 && equation != "0")//只有一个运算数且不是0
        {
            addAndChange_num("0");
            break;
        }
        else if(equation[i] == '.')//如果前面有小数点，则无条件允许输入
        {
            addAndChange_num("0");
            break;
        }
        else if(is_signal(equation[i]))//遇到运算符
        {
            if(equation[i+1] != '0')//运算符后面的第一个数不为0则可以输入0
            {
                addAndChange_num("0");
                break;
            }
        }
    }
}

void calculator::btn_1()
{
    if(equation.size() == MAX_BOTTOM_LENGTH)
        return;
    check_erase_0();
    clear_top();
    addAndChange_num("1");
}

void calculator::btn_2()
{
    if(equation.size() == MAX_BOTTOM_LENGTH)
        return;
    check_erase_0();
    clear_top();
    addAndChange_num("2");
}

void calculator::btn_3()
{
    if(equation.size() == MAX_BOTTOM_LENGTH)
        return;
    check_erase_0();
    clear_top();
    addAndChange_num("3");
}

void calculator::btn_4()
{
    if(equation.size() == MAX_BOTTOM_LENGTH)
        return;
    check_erase_0();
    clear_top();
    addAndChange_num("4");
}

void calculator::btn_5()
{
    if(equation.size() == MAX_BOTTOM_LENGTH)
        return;
    check_erase_0();
    clear_top();
    addAndChange_num("5");
}

void calculator::btn_6()
{
    if(equation.size() == MAX_BOTTOM_LENGTH)
        return;
    check_erase_0();
    clear_top();
    addAndChange_num("6");
}

void calculator::btn_7()
{
    if(equation.size() == MAX_BOTTOM_LENGTH)
        return;
    check_erase_0();
    clear_top();
    addAndChange_num("7");
}

void calculator::btn_8()
{

    if(equation.size() == MAX_BOTTOM_LENGTH)
        return;
    check_erase_0();
    clear_top();
    addAndChange_num("8");
}

void calculator::btn_9()
{
    if(equation.size() == MAX_BOTTOM_LENGTH)
        return;
    check_erase_0();
    clear_top();
    addAndChange_num("9");
}

/*功能按键*/
void calculator::btn_pls()
{
    if(equation.size() == MAX_BOTTOM_LENGTH)
        return;
    inherit_last_result();
    check_erase_signal();
    check_erase_point();
    if(equation == "0")//充当正负号
    {
        equation.clear();
        equation_for_show.clear();
    }
    clear_top();
    addAndChange_num("+");
}

void calculator::btn_sub()
{
    if(equation.size() == MAX_BOTTOM_LENGTH)
        return;
    inherit_last_result();
    check_erase_signal();
    check_erase_point();
    if(equation == "0")//充当正负号
    {
        equation.clear();
        equation_for_show.clear();
    }
    clear_top();
    addAndChange_num("-");
}

void calculator::btn_mul()
{
    if(equation.size() == MAX_BOTTOM_LENGTH)
        return;
    inherit_last_result();
    check_erase_signal();
    check_erase_point();
    clear_top();
    equation_for_show += "×";
    equation += "*";
    ui->bottom_output->setText(equation_for_show);
}

void calculator::btn_div()
{
    if(equation.size() == MAX_BOTTOM_LENGTH)
        return;
    inherit_last_result();
    check_erase_signal();
    check_erase_point();
    clear_top();
    equation_for_show += "÷";
    equation += "/";
    ui->bottom_output->setText(equation_for_show);
}

void calculator::btn_point()
{
    inherit_last_result();
    for(int i=equation.size()-1;i>=0;i--)//检测当前数中是否已经有了小数点
        if(equation[i] == '.')//如果有，不响应
            return;
        else if(is_signal(equation[i]))//如果遇到运算符或者到0，则可以继续
            break;

    if(equation.size() == MAX_BOTTOM_LENGTH)
        return;
    else if(equation.size() == 0)
    {
        equation_for_show += "0";
        equation+="0";
    }
    else if(equation[equation.size()-1]>'9' || equation[equation.size()-1]<'0' || is_signal(equation[equation.size()-1]))
    {
        equation_for_show += "0";
        equation+="0";
    }
    clear_top();
    equation_for_show += ".";
    equation += ".";
    ui->bottom_output->setText(equation_for_show);
}

void calculator::btn_left()
{
    if(equation.size() == MAX_BOTTOM_LENGTH)
        return;
    inherit_last_result();
    check_erase_0();
    clear_top();
    equation_for_show += "(";
    equation += "(";
    ui->bottom_output->setText(equation_for_show);
}

void calculator::btn_right()
{
    if(equation.size() == MAX_BOTTOM_LENGTH)
        return;
    inherit_last_result();
    if(equation == "0")
    {
        equation_for_show.resize(equation.size()-1);
        equation.resize(equation.size()-1);
    }
    else if(equation_for_show[equation.size()-1] == '(')
    {
        equation_for_show += "0";
        equation += "0";
    }
    clear_top();
    equation_for_show += ")";
    equation += ")";
    ui->bottom_output->setText(equation_for_show);
}

void calculator::btn_equal()
{
    double result = 0;
    try{
        if(equation != "0")
        {
            result = calculate(equation);
            ui->bottom_output->setText(QString::number(result ,'g',PRECISION));
            ui->top_output->setText(equation_for_show + "=");
            clear_equation();
            last_result = result;
        }
        else
        {
            clear_top();
            if(last_signal == '+')
            {
                last_result+=last_num;
            }
            else if(last_signal == '-')
            {
                last_result-=last_num;
            }
            else if(last_signal == '*')
            {
                last_result*=last_num;
            }
            else if(last_signal == '/')
            {
                last_result/=last_num;
            }
            ui->top_output->setText(QString("PREV")+last_signal+QString::number(last_num,'g',PRECISION));
            ui->bottom_output->setText(QString::number(last_result,'g',PRECISION));
        }
    }
    catch(const char * c)
    {
        ui->bottom_output->setText(QString(c));
        clear_equation();
    }
}
//键盘读入
void calculator::keyPressEvent(QKeyEvent * e)
{
    switch(e->key())
    {
        case '0':
        {
            btn_0();
            break;
        }
        case '1':
        {
            btn_1();
            break;
        }
        case '2':
        {
            btn_2();
            break;
        }
        case '3':
        {
            btn_3();
            break;
        }
        case '4':
        {
            btn_4();
            break;
        }
        case '5':
        {
            btn_5();
            break;
        }
        case '6':
        {
            btn_6();
            break;
        }
        case '7':
        {
            btn_7();
            break;
        }
        case '8':
        {
            btn_8();
            break;
        }
        case '9':
        {
            btn_9();
            break;
        }
        case '+':
        {
            btn_pls();
            break;
        }
        case '-':
        {
            btn_sub();
            break;
        }
        case '*':
        {
            btn_mul();
            break;
        }
        case '/':
        {
            btn_div();
            break;
        }
        case '.':
        {
            btn_point();
            break;
        }
        case 16777220:
        case 16777221:
        {
            btn_equal();
            break;
        }
        case 16777219:
        {
            btn_del();
            break;
        }
        case '(':
        {
            btn_left();
            break;
        }
        case ')':
        {
            btn_right();
            break;
        }
    }
}
//计算函数
double calculator::calculate(QString & equation)
{
    const QChar MULTIPLY = '*';
    const QChar DIVIDE ='/';
    const QChar PLUS = '+';
    const QChar SUBTRACK = '-';
    /*查找是否有省略乘号情况*/
    int pos = 0;
    while (pos != -1)
    {
        if (pos > 0)
            if ((equation[pos - 1] >= '0' && equation[pos - 1] <= '9') || equation[pos - 1] == ')')
            {
                equation.insert(pos, '*');
                pos += 2;
            }
        pos++;
        pos = equation.indexOf('(', pos);
    }
    pos = 0;
    while (pos != -1)
    {
        if (pos > 0 && pos<equation.size() - 1)
            if ((equation[pos + 1] >= '0' && equation[pos + 1] <= '9') || equation[pos + 1] == '(')
            {
                equation.insert(pos+1, '*');
                pos += 2;
            }
        pos++;
        pos = equation.indexOf(')', pos);
    }

    /*缺失最后一个操作数则抛出错误*/
    if(is_signal(equation[equation.size()-1]))
        throw "缺少运算数";

    /*如果只输入小数点没有小数，则认为没有输入小数点*/
    check_erase_point();

    /*如果第一个字符为+则去掉*/
    if(equation[0] == '+')
    {
        equation.remove(0,1);
        equation_for_show.remove(0,1);
    }

    /*根据算式中是否还有括号判断不同的路径*/
    auto first_left = equation.indexOf('(');
    auto last_right = equation.lastIndexOf(')');
    if (first_left == last_right)//如果算式当中已经没有括号
    {
        double result;
        double num1, num2, temp_result;
        QVector<QChar> sigs;
        QVector<double> nums;
        QString current_num;
        for (auto i : equation)
            if (i == MULTIPLY || i == DIVIDE || i == PLUS || i == SUBTRACK)
            {
                if (current_num.size() == 0)
                {
                    if (i == PLUS || i == SUBTRACK)
                        current_num.push_back(i);
                    else
                        throw "运算符错误";
                }
                else
                {
                    sigs.push_back(i);
                    nums.push_back(current_num.toDouble());
                    current_num.clear();
                }
            }
            else if ((i >= '0'&&i <= '9') || i == '.')
            {
                current_num += i;
            }
        nums.push_back(current_num.toDouble());//最后一个数

        for (int i = 0; i < sigs.size(); i++)
        {
            if (sigs[i] == MULTIPLY)
            {
                num1 = nums[i];
                num2 = nums[i + 1];
                temp_result = num1*num2;
                last_num = nums[i+1];
                last_signal = sigs[i];
                nums.remove(i, 2);
                sigs.remove(i);
                nums.insert(nums.begin() + i, temp_result);
                i--;
            }
            else if (sigs[i] == DIVIDE)
            {
                num1 = nums[i];
                num2 = nums[i + 1];
                temp_result = num1 / num2;
                last_num = nums[i+1];
                last_signal = sigs[i];
                nums.remove(i, 2);
                sigs.remove(i);
                nums.insert(nums.begin() + i, temp_result);
                i--;
            }
        }
        result = nums[0];
        for (int i = 0; i < nums.size() - 1; i++)
            if (sigs[i] == PLUS)
                result += nums[i + 1];
            else if (sigs[i] == SUBTRACK)
                result -= nums[i + 1];
        if(nums.size()>1)//如果还有加减法运算
        {
            last_num = nums[nums.size() - 1];//记录最后一个操作数
            last_signal = sigs[nums.size() - 2];//记录最后一个操作运算符
        }
        return result;
    }
    else if (first_left == -1)//括号不配对情况
    {
        //equation.remove(equation.lastIndexOf(')'), 1);
        throw "括号不成对";
    }
    else if (last_right == -1)
    {
        //equation.remove(equation.indexOf('('), 1);
        throw "括号不成对";
    }
    else//抹掉括号
    {
        int left, right, left_2, right_2;
        left = equation.indexOf('(');
        right = equation.lastIndexOf(')');
        left_2 = equation.lastIndexOf('(');
        right_2 = equation.indexOf(')');
        if (left_2 >= left && right_2 <= right && left_2 < right_2)//只有嵌套括号
        {
            QString sub_str = equation.mid(first_left + 1, last_right - first_left - 1);
            double sub_result = calculate(sub_str);
            equation.replace(first_left, last_right - first_left + 1, QString::number(sub_result,'g',PRECISION));
            return calculate(equation);
        }
        else//有分离括号
        {
            left = equation.lastIndexOf('(');//找到最右边的左括号
            right = equation.mid(left, equation.size()).indexOf(')') + left;//最右边的左括号后的最左边的右括号
            //此时left和right之间有单位算式
            QString sub_str = equation.mid(left + 1, right - left - 1);
            double sub_result = calculate(sub_str);
            equation.replace(left, right - left + 1, QString::number(sub_result,'g',PRECISION));
            return calculate(equation);
        }
    }
}
//判断是否为运算符
static bool is_signal(QChar c)
{
    return (c=='+'||c=='-'||c=='*'||c=='/');
}
