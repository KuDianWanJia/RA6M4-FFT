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

### 软件框架
1、软件方面开了许多个线程，中断和线程、线程和线程之间通过队列传递数据。

2、RT-Thread的使用
RTT是个很优秀的操作系统，我在我的项目中使用了RTT作为RTOS的线程、队列等操作系统的实现
RTT有一个对板级适配的HAL层，我在我的项目中SPI、UART及UART中断、GPIO使用的都是RTT提供的HAL层的函数，增强了可移植性。
RTT还有社区和软件包的适配，本次我使用的三方库都是适配了RTT的版本，只是从RT-Thread Settings内开启即可使用，大大加快了开发途中寻找、适配三方库的速度。

## 实物照片

![1.jpg](./图片1.jpg)

![2.jpg](./图片2.jpg)

## 仓库地址
https://github.com/KuDianWanJia/RA6M4-TFT

## 视频地址
https://www.bilibili.com/video/BV17a411T72y/
