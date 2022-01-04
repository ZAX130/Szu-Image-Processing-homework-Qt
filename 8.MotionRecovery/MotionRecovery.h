#pragma once

#include <QtWidgets/QWidget>
#include "ui_MotionRecovery.h"

class MotionRecovery : public QWidget
{
    Q_OBJECT

public:
    MotionRecovery(QWidget *parent = Q_NULLPTR);

private:
    Ui::MotionRecoveryClass ui;
};
