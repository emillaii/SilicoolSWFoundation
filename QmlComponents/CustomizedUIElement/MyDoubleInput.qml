import QtQuick 2.0
import QtQuick.Controls 2.12

TextField{
    validator: DoubleValidator{
        notation: DoubleValidator.StandardNotation
    }
    implicitWidth: 80
    selectByMouse: true
    horizontalAlignment: TextField.AlignHCenter
}

