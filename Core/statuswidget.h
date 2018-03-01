#ifndef STATUSWIDGET_H
#define STATUSWIDGET_H

#include <ErrorLabel.h>

#include <QProgressBar>
#include <QWidget>

namespace Ui {
class StatusWidget;
}

class StatusWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StatusWidget(QWidget *parent = 0);
    ~StatusWidget();

    void showErrorLabel();
    void showProgressBar();

    RsaToolbox::ErrorLabel *errorLabel();
    QProgressBar *progressBar();

private:
    Ui::StatusWidget *ui;
};

#endif // STATUSWIDGET_H
