错误处理模块

1. 您需要熟悉std::function, std::bind, 及lamda表达式，以方便地传递函数对象

2. 可被处理的错误，请抛出SilicoolError的派生类对象。无法被处理的错误，请抛出SilicoolAbort对象。
	所有的SilicoolError派生类对象需要被程序员处理（因您知道如何处理该错误），SilicoolAbort由框架处理（例如，手动模式下，使用TaskEngineeManager执行一串指令，中间某个指令抛出SilicoolAbort错误，后续指令均不会被执；自动模式下，会退出自动模式，进入Abort状态，需要重新初始化整机才能继续运行)。

3. 若抛出SilicoolError错误的函数，能处理该错误，就尽量让本函数处理，否则请声明该函数可能抛出此错误，以便让所有调用者处理该错误

4. 您可以自定义SilicoolError派生类用于抛出错误，自定义ErrorHandler派生类处理该错误。
	预定义的错误处理选项如下：
	Retry： 重新执行函数对象。请确保重新执行函数对象不会造成不期望的行为，否则该错误不能被Retry处理。
	Ignore: 忽略此错误，该函数对象中，发生此错误的后续代码不会被执行。请确保该错误可以被忽略。
	Abort：抛出SilicoolAbort错误，将错误交付框架处理。
	更多使用示例可参考ErrorHandlingTest项目