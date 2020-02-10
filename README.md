# ZBarUtils
 针对zbar扫码的重新编译与封装，去掉Android中不需要的冗余的代码，简化开发者进行调用。

---
### 简单使用
- Step01：进行扫码操作

	```Kotlin
	// 扫码方法一，适用于经过裁剪情况下扫码
	external fun scanImage(
	        data: ByteArray,
	        width: Int,
	        height: Int,
	        cropX: Int,
	        cropY: Int,
	        cropWidth: Int,
	        cropHeight: Int
	    ): Int
	    
	// 扫码方法二，适用于不进行裁剪情况下扫码    
	fun scanImage(data: ByteArray, width: Int, height: Int): Int
	```
	- 参数说明：

		参数 | 参数类型 | 参数说明
		--- | --- | ---
		`data` 		| `byte[]`   | 图像数据，Android 中一般是 `NV21` 格式
		`width`		| `int`		| 图像的宽
		`height`		| `int`		| 图像的高
		`cropX` 		| `int`		| 裁剪的起始横坐标
		`cropY`		| `int`		| 裁剪的起始纵坐标
		`cropWidth`	| `int`		| 裁剪的宽
		`cropHeight`	| `int`		| 裁剪的高
 
	- 返回值说明：<font color=red>返回值大于 0 时，表示扫码获取到了结果</font>

- Step02：获取扫码结果

	```Kotlin
	external fun obtainResult(): Array<String>?
	```
	- 当扫码不成功时获取的结果为 `null`
	- 返回的为扫码识别的结果

- Step03：释放操作

	```Kotlin
	external fun destroy()
	```
	当不需要再使用的时候，调用该方法进行相关释放操作，以防内存泄漏。


---

## License
```
Copyright 2020 Yanjun Xu

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
```