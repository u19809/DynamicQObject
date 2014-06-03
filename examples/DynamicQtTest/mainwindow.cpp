#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "testobject.h"
#include "dynamictestobject.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    TO= new TestObject( this );
    
    connect( TO, SIGNAL( textChanged(const QString & ) ), 
             ui->PropertyValue_LBL, SLOT( setText( const QString & ) ) );
    
    connect( this, SIGNAL( updateProperty(const QString &)), 
             TO, SLOT( setTxt( const QString & ) ) );

    DynamicMetaObject::dump( TO->metaObject() );
        
    DTO = new DynamicTestObject( this );
    
    connect( DTO, SIGNAL( textChanged(const QString & ) ), 
             ui->PropertyValue2_LBL, SLOT( setText( const QString & ) ) );
    
    connect( this, SIGNAL( updateProperty2(const QString &)), 
             DTO, SLOT( setTxt( const QString & ) ) );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_InvokeMethod_PB_clicked() {
}

void MainWindow::on_InvokeSlot_PB_clicked(){
    emit updateProperty( ui->InvokeSlot_LE->text() );
}

void MainWindow::on_SetProperty_PB_clicked(){
    TO->setProperty( "Txt", ui->SetProperty_LE->text() );
}

void MainWindow::on_GetProperty_PB_clicked(){
    ui->GetProperty_LBL->setText( TO->property( "Txt" ).toString() );
}

void MainWindow::on_InvokeMethod2_PB_clicked(){
    DTO->someMethod( ui->InvokeMethod2_LE->text() );
}

void MainWindow::on_InvokeSlot2_PB_clicked(){
    emit updateProperty2( ui->InvokeSlot2_LE->text() );
}

void MainWindow::on_SetProperty2_PB_clicked(){
    DTO->setProperty( "Txt", ui->SetProperty2_LE->text() );
}

void MainWindow::on_GetProperty2_PB_clicked(){
    ui->GetProperty2_LBL->setText( DTO->property( "Txt" ).toString() );
}

