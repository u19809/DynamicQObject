#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class TestObject;
class DynamicTestObject;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
public slots :
    
    void on_InvokeMethod_PB_clicked();
    void on_InvokeMethod2_PB_clicked();
    void on_InvokeSlot_PB_clicked();
    void on_InvokeSlot2_PB_clicked();
    void on_GetProperty_PB_clicked();
    void on_GetProperty2_PB_clicked();
    void on_SetProperty_PB_clicked();
    void on_SetProperty2_PB_clicked();
    
signals :
    
    void updateProperty( const QString & Txt );
    void updateProperty2( const QString & Txt );
    
private:
    Ui::MainWindow *ui;
    
    TestObject * TO;
    DynamicTestObject * DTO;
};

#endif // MAINWINDOW_H
