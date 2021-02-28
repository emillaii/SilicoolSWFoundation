LocalFile<--->Model <--->UI自动同步
支持undo，redo，更改配置时自动保存到文件
加密保存文件
Log保存更改记录
根据Model中相关定义，生成简单编辑界面。（可任意嵌套）（生成Label，TextField，CheckBox，ComboBox，是否在UI显示，是否只读等）
主机可将配置通过网络共享出去，从机可订阅该配置，当主机配置改变时，自动发布通知，从机自动更新自己的配置。

---------------警告--------------------
若您定义类型派生自ConfigBase，该类型中嵌套包含其它ConfigBase派生类对象，请确保设置被嵌套包含的对象的parent为this。否则会由于QT的垃圾回收机制，当在UI对ConfigObjectArray执行添加和删除操作后，不断执行撤销和重做导致程序崩溃。

举个例子：
class A: public ConfigObject
class B: public ConfigObject	
{
	Property(A* a)	//类型B中包含A的对象
}
class C: public ConfigObject
{
	Property(ConfigObjectArray* bArray)	//类型C中包含类型B的Array
}
使用C的对象自动创建UI，在UI上对bArray执行一些增加及删除，然后执行一些撤销和重做，会导致程序崩溃。
因为当移除bArray中某个对象时，该对象不会被删除，因为框架有自动设置bArray中的对象的parent，
但是被移除的bArray的对象，即某个类B的实例，该实例中的对象a会被QT回收。。。（可能由于对应的qml元素销毁），此时再执行撤销操作，程序就会崩溃。

解决办法：将a的parent设为B类对象的this。
