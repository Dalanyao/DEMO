# TuyaOS TuyaBeacon Platform phy6252

更新时间：2022-04-11  作者：尼亚

## 搭建环境

### 搭建硬件环境

#### 芯片手册

待补充

tuyaos io定义：

![image_33186930556712335.png](https://github.com/wangwei573979071/photo-gallery-/blob/main/image_33186930556712335.png?raw=true)

### 核心板

![企业微信截图_16479398874723.png](https://github.com/wangwei573979071/photo-gallery-/blob/main/%E4%BC%81%E4%B8%9A%E5%BE%AE%E4%BF%A1%E6%88%AA%E5%9B%BE_16479398874723.png?raw=true)

### 底板

待补充……

### 启动

#### 启动流程

待补充……

#### 启动时间

![企业微信截图_1647940156211.png](https://github.com/wangwei573979071/photo-gallery-/blob/main/%E4%BC%81%E4%B8%9A%E5%BE%AE%E4%BF%A1%E6%88%AA%E5%9B%BE_1647940156211.png?raw=true)

### 功耗

待补充……

### 存储

![image_17464733597971271.png](https://images.tuyacn.com/fe-static/docs/img/3fe2af1a-65f1-402f-a4f0-57198d0fb608.png)

### 外设

#### UART

芯片默认支持两个串口（UART0、UART1）

烧录授权：

| **UART**  | **功能** | **引脚** |
| --------- | -------- | -------- |
| **UART**0 | TX       | P8       |
| **UART**0 | RX       | P9       |

LOG:

| **UART**  | **功能** | **引脚** |
| --------- | -------- | -------- |
| **UART**1 | TX       | P0       |
| **UART**1 | RX       | P2       |

#### 

#### SPI

默认一路，SPI1

| **SPI** | **功能** | **引脚** |
| ------- | -------- | -------- |
| SPI1    | CS       | P32      |
| SPI1    | CLK      | P31      |
| SPI1    | SDI      | P20      |
| SPI1    | SDO      | P18      |

#### IIC

默认一路，I2C0

| **IIC** | **功能** | **引脚** |
| ------- | -------- | -------- |
| IIC0    | SCL      | P34      |
| IIC0    | SDA      | P33      |

#### PWM

支持6路。

| **通道** | **引脚** |
| -------- | -------- |
| 0        | P14      |
| 1        | P2       |
| 2        | P16      |
| 3        | P3       |
| 4        | P11      |
| 5        | P7       |



#### ADC

支持5路。

| **通道** | **引脚** |
| -------- | -------- |
| 0        | P11      |
| 1        | P23      |
| 2        | P24      |
| 3        | P14      |
| 4        | P15      |

#### 上电启动检测引脚

P7
