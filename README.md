# 基于RT-Thread+RA6M4的FFT音乐频谱

## 项目介绍
本项目制作了一个FFT音乐频谱显示器，ADC采集音频信号进行FFT运算，通过1.5'OLED进行显示，同时实现了OLED动画效果的显示

## 应用背景
各种音乐频谱灯、频谱仪、氛围灯

## 实现功能
1.5寸彩色OLED并口驱动
简单GUI，可实现多样的GUI动画效果
音频信号ADC采集并进行FFT运算,获取频谱信息进行显示

## 整体框架
### 硬件框架
![硬件框图.png](./%E7%A1%AC%E4%BB%B6%E6%A1%86%E5%9B%BE.png)

### 软件框架
![软件框图.png](./%E8%BD%AF%E4%BB%B6%E6%A1%86%E5%9B%BE.png)

## RT-Thread的使用
本次项目中使用了RTT操作系统，对RA6M4开发板绝大多数常用接口如UART、GPIO、ADC、IIC等进行了适配，但也有一部分还没适配，待后期更新，所以软件的开发以RTT API结合瑞萨官方BSP API进行，实现硬件定时器触发ADC采集，RTT线程定时进行FFT运算，获取频谱，并通过OLED显示频谱数据。
RTT有一个对板级适配的HAL层，对多种型号的MCU进行了BSP适配，同时还适配了许多第三方软件包，能大大加快项目的开发进度，增强了代码的可移植性。

## 实物照片

![1.jpg](./%E5%9B%BE%E7%89%871.jpg)

![2.jpg](./%E5%9B%BE%E7%89%872.jpg)

## 仓库地址
https://github.com/KuDianWanJia/RA6M4-TFT

## 视频地址
https://www.bilibili.com/video/BV17a411T72y/
