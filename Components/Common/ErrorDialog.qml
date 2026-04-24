import QtQuick 2.15
import QtQuick.Dialogs

MessageDialog{
         property string errorText: ""
         // id:errorDialg
         title:"错误"
         text:""
         informativeText: errorText
         buttons:MessageDialog.Ok
}
