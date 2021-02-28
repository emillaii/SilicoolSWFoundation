#ifndef BTNNAMEDEFINITION_H
#define BTNNAMEDEFINITION_H

#include <QString>

const QString YesBtn = "Yes";
const QString NoBtn = "No";
const QString PassBtn = "Pass";
const QString FailBtn = "Fail";
const QString OkBtn = "Ok";
const QString CancelBtn = "Cancel";

const QString BtnSeperator = "|";

const QString YesNoBtns = YesBtn + BtnSeperator + NoBtn;
const QString PassFailBtns = PassBtn + BtnSeperator + FailBtn;
const QString OkCancelBtns = OkBtn + BtnSeperator + CancelBtn;

#endif    // BTNNAMEDEFINITION_H
