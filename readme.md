### Bad Apple - STM32 OLED版本

---

BadApple.mp4是原版高清视频

VideoToBin.m是处理视频/处理图片的MATLAB代码

images.zip是视频抽帧后保存的图片压缩包

BadApple.bin是处理图片后保存的bin文件

FATFS是将W25Q128格式化为FATFS的工程

USB是将bin文件写入W25Q128的工程

BadApple是让OLED显示动画的工程

README.md是这个文本文件

---

使用步骤：

1. 用FATFS工程将W25Q128格式化为FATFS文件系统（如果W25Q128已经有FATFS了则跳过此步骤）

2. 用USB工程将bin文件从电脑导入W25Q128

3. 连接好电路，运行BadApple工程

---

详情请见： https://zhuanlan.zhihu.com/p/198400688
