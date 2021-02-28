import QtQuick 2.0

Item {
    property var nameIndexMap: []
    property alias itemModel: itemModel

    ListModel{
        id: itemModel
    }

    function initItemModel(itemNames, createItemFunction){
        for(var i = 0; i < itemNames.length; i++){
            var obj = createItemFunction(itemNames[i])
            itemModel.append(obj)
            nameIndexMap[itemNames[i]] = i
        }
    }
}
