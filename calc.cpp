#include "calc.h"
#include <QRegularExpression>
#include "ui_calc.h"

// Holds current value of calculation
double calcVal = 0.0;

// Will define if this was the last math button clicked
bool divTrigger = false;
bool multTrigger = false;
bool addTrigger = false;
bool subTrigger = false;

// Constructor
Calc::Calc(QWidget *parent) :

    // Call the QMainWindow constructor
    QMainWindow(parent),

    // Create the UI class and assign it to the ui member
    ui(new Ui::Calc)
{

    // Setup the UI
    ui->setupUi(this);

    // Put 0.0 in Display
    ui->Display->setText(QString::number(calcVal));

    // Will hold references to all the number buttons
    QPushButton *numButtons[10];

    // Cycle through locating the buttons
    for(int i = 0; i < 10; ++i){
        QString butName = "Button" + QString::number(i);

        // Get the buttons by name and add to array
        numButtons[i] = Calc::findChild<QPushButton *>(butName);

        // When the button is released call num_pressed()
        connect(numButtons[i], SIGNAL(released()), this,
                SLOT(NumPressed()));
    }

    // Connect signals and slots for math buttons
    connect(ui->Add, SIGNAL(released()), this,
            SLOT(MathButtonPressed()));
    connect(ui->Subtract, SIGNAL(released()), this,
            SLOT(MathButtonPressed()));
    connect(ui->Multiply, SIGNAL(released()), this,
            SLOT(MathButtonPressed()));
    connect(ui->Divide, SIGNAL(released()), this,
            SLOT(MathButtonPressed()));

    // Connect equals button
    connect(ui->Equals, SIGNAL(released()), this,
            SLOT(EqualButtonPressed()));

    // Connect change sign
    connect(ui->ChangeSign, SIGNAL(released()), this,
            SLOT(ChangeNumberSign()));
    // Connect clear button
    connect(ui->ButtonClear, SIGNAL(released()), this, SLOT(ClearDisplay()));

}

Calc::~Calc()
{
    delete ui;
}

void Calc::NumPressed(){

    // Sender returns a pointer to the button pressed
    QPushButton *button = (QPushButton *)sender();

    // Get number on button
    QString butVal = button->text();

    // Get the value in the display
    QString displayVal = ui->Display->text();

    if((displayVal.toDouble() == 0) || (displayVal.toDouble() == 0.0)){

        // calcVal = butVal.toDouble();
        ui->Display->setText(butVal);

    } else {
        // Put the new number to the right of whats there
        QString newVal = displayVal + butVal;

        // Double version of number
        double dblNewVal = newVal.toDouble();

        // calcVal = newVal.toDouble();

        // Set value in display and allow up to 16
        // digits before using exponents
        ui->Display->setText(QString::number(dblNewVal, 'g', 16));

    }
}

void Calc::MathButtonPressed(){

    // Cancel out previous math button clicks
    divTrigger = false;
    multTrigger = false;
    addTrigger = false;
    subTrigger = false;

    // Store current value in Display
    QString displayVal = ui->Display->text();
    calcVal = displayVal.toDouble();

    // Sender returns a pointer to the button pressed
    QPushButton *button = (QPushButton *)sender();

    // Get math symbol on the button
    QString butVal = button->text();

    if(QString::compare(butVal, "/", Qt::CaseInsensitive) == 0){
        divTrigger = true;
    } else if(QString::compare(butVal, "*", Qt::CaseInsensitive) == 0){
        multTrigger = true;
    } else if(QString::compare(butVal, "+", Qt::CaseInsensitive) == 0){
        addTrigger = true;
    } else {
        subTrigger = true;
    }

    // Clear display
    ui->Display->setText("");

}

void Calc::EqualButtonPressed(){

    // Holds new calculation
    double solution = 0.0;

    // Get value in display
    QString displayVal = ui->Display->text();
    double dblDisplayVal = displayVal.toDouble();

    // Make sure a math button was pressed
    if(addTrigger || subTrigger || multTrigger || divTrigger ){
        if(addTrigger){
            solution = calcVal + dblDisplayVal;
        } else if(subTrigger){
            solution = calcVal - dblDisplayVal;
        } else if(multTrigger){
            solution = calcVal * dblDisplayVal;
        } else {
            solution = calcVal / dblDisplayVal;
        }
    }

    // Put solution in display
    ui->Display->setText(QString::number(solution));

}

void Calc::ChangeNumberSign(){

    // Get the value in the display
    QString displayVal = ui->Display->text();

    // Regular expression checks if it is a number
    // plus sign
    QRegularExpression reg("[-+]?[0-9.]*");

    // If it is a number change the sign
    if(reg.match(displayVal).hasMatch()){
        double dblDisplayVal = displayVal.toDouble();
        double dblDisplayValSign = -1 * dblDisplayVal;

        // Put solution in display
        ui->Display->setText(QString::number(dblDisplayValSign));
    }

}

void Calc::ClearDisplay() {
    ui->Display->clear(); // Assuming ui->Display is the display widget
}
