### Bad Apple - STM32 OLED 版本

---

- `BadApple/` 是让 OLED 显示动画的工程
- `FATFS/` 是将 W25Q128 格式化为 FATFS 的工程
- `USB/` 是将 bin 文件写入 W25Q128 的工程
- `BadApple.mp4` 是原版高清视频
- `VideoToBin.m` 是处理视频/处理图片的 MATLAB 代码
- `readme.md` 是这个文本文件

---

使用步骤：

1. 用 FATFS 工程将 W25Q128 格式化为 FATFS 文件系统（如果 W25Q128 已经有 FATFS 了则跳过此步骤）
2. 用 USB 工程将 bin 文件从电脑导入 W25Q128
3. 连接好电路，运行 BadApple 工程

---

详情请见： https://zhuanlan.zhihu.com/p/198400688
