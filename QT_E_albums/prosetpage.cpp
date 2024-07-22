#include "prosetpage.h"
#include "ui_prosetpage.h"

ProSetPage::ProSetPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::ProSetPage)
{
    ui->setupUi(this);
    //将两个lineEdit注册为wizard的field，保证两个lineEdit是空的时候无法点击下一步
    registerField("proName",ui->lineEdit);
    registerField("proPath",ui->lineEdit_2);
    //将QLineEdit的textEdited信号和ProSetPage的completeChanged信号连接起来，
    //这样在lineEdit编辑的时候就会发送textEdited信号，进而触发ProSetPage发送completeChanged信号。
    connect(ui->lineEdit,&QLineEdit::textEdited,this,&ProSetPage::completeChanged);
    connect(ui->lineEdit_2,&QLineEdit::textEdited,this,&ProSetPage::completeChanged);

    ui->lineEdit->setText("Input name");
    ui->lineEdit->setClearButtonEnabled(true);
    //获取当前路径
    QString curPath = QDir::currentPath();
    //将当前路径作为默认值放到text里
    ui->lineEdit_2->setText(curPath);
    ui->lineEdit_2->setCursorPosition(ui->lineEdit_2->text().size());
    //添加一键删除按钮
    ui->lineEdit_2->setClearButtonEnabled(true);
}

ProSetPage::~ProSetPage()
{
    delete ui;
}

void ProSetPage::GetProSettings(QString &name, QString &path)
{
    name = ui->lineEdit->text();
    path = ui->lineEdit_2->text();
}

//重构虚函数 isComplete 判断页面是否更改完成
//能判断文件夹是否合理以及是否已经有项目路径了。
//可以根据不满足的条件设置tips提示用户。
bool ProSetPage::isComplete() const
{
    if(ui->lineEdit->text() == "" || ui->lineEdit_2->text() == ""){
        return false;
    }
    QDir dir(ui->lineEdit_2->text());
    if(!dir.exists()){
        ui->tips->setText("project path is not extits");
        return false;
    }
    QString absFilePath = dir.absoluteFilePath(ui->lineEdit->text());
    QDir dist_dir(absFilePath);
    if(dist_dir.exists()){
        ui->tips->setText("project has exits,change path or name!");
        return false;
    }
    ui->tips->setText("ok!");
    //自己实现qt的虚函数的时候,为了防止原本功能的被覆盖，这里需要调用旧的函数
    return QWizardPage::isComplete();
}
void ProSetPage::on_pushButton_clicked()
{
    QFileDialog file_dialog;
    file_dialog.setFileMode(QFileDialog::Directory);
    file_dialog.setWindowTitle("选择导入的文件夹");
    auto path = QDir::currentPath();
    file_dialog.setDirectory(path);
    file_dialog.setViewMode(QFileDialog::Detail);

    QStringList fileNames;
    if(file_dialog.exec()){
        fileNames = file_dialog.selectedFiles();
    }
    if(fileNames.size() < 0){
        return ;
    }
    QString import_path = fileNames.at(0);
    qDebug()<<"Imort_path is "<< import_path << endl;
    ui->lineEdit_2->setText(import_path);
}
