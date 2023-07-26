# REMCU examples

Examples illustrating application development on the [**REMCU Lib**](https://remotemcu.com/). These examples are designed to get you up and running with REMCU Lib in no time.

| Target 	| Jupyter notebook ex. 	| Python scripts ex. 	| Qt app ex. 	| C/C++ app ex. 	| Raspberry Pi board examples 	|
|:-----------------:	|:--------------------------------------------------:	|:------------------:	|:-------------------------------:	|:--------------------:	|:------------------------------------------------------------------------------------------------:	|
| stm32f4_discovery 	| C++(cling) & Python:<br>*ADC, DAC, DMA, GPIO, PWM* 	|  	| Accelerometer & CAN bus sniffer 	|  	|  	|
| STM32F767X 	| C++(cling): *ADC, DAC, DMA, GPIO, PWM* 	|  	|  	|  	|  	|
| STM32F103RCT6 	| C++(cling) & Python: *ADC, GPIO, PWM* 	| *ADC, GPIO* 	|  	| *ADC, DAC, DMA, GPIO, CAN bus* 	| C/C++ apps & Python scripts: *ADC, GPIO, CAN bus*<br> - the prepared Raspbian image is available 	|
| STM32F103C8T6<br>Blue Pill board 	| C++(cling) & Python: *ADC, GPIO* 	| *ADC, GPIO* 	|  	| ADC, CAN Bus 	| <br>C/C++ apps & Python scripts: *ADC, GPIO, CAN bus*<br> - the prepared Raspbian image will be available 	|
| stm8l_discovery 	| C++(cling): *ADC, DAC, DMA, GPIO, LCD screen* 	|  	|  	|  	|  	|
| STM32F030-51 	| C++(cling) & Python: *ADC, GPIO* 	|  	|  	|  	| Python scripts: *ADC, GPIO*<br> - the prepared Raspbian image is available 	|
| stm32f3_discovery 	| soon 	|  	| Compas & Gyroscope 	|  	|  	|
| KIT_XMC_2GO_XMC1100 	| WIP 	| soon 	|  	| [uart](https://github.com/remotemcu/remcu-chip-sdks/blob/master/infineon/XMC/XMC_Peripheral_Library_v2.1.18/test/uart.c) 	|  	|
| LPC1768(NXP) 	| soon 	|  	| soon: CAN bus sniffer 	| [adc](https://github.com/remotemcu/remcu-chip-sdks/blob/master/NXP/LPC_17XX_40XX/CMakeLists/LPC175X_6X/test/adc.c), [dac](https://github.com/remotemcu/remcu-chip-sdks/blob/master/NXP/LPC_17XX_40XX/CMakeLists/LPC175X_6X/test/dac.c), [gpio](https://github.com/remotemcu/remcu-chip-sdks/blob/master/NXP/LPC_17XX_40XX/CMakeLists/LPC175X_6X/test/gpio.c) 	|  	|
| FRDM-K64F<br>MK64FN1M0VLL12 MCU<br>(Freescale/NXP) 	| soon 	|  	| soon: Accelerometer 	| [gpio](https://github.com/remotemcu/remcu-chip-sdks/tree/master/freescale/KSDK_1.3/CMakeLists/MK64FN1M0VMD12/test/gpio), [mma8451 accelerometer I2C](https://github.com/remotemcu/remcu-chip-sdks/tree/master/freescale/KSDK_1.3/CMakeLists/MK64FN1M0VMD12/test/accel_i2c) 	|  	|
| EFM32TG(Silabs) 	| WIP 	|  	| soon: LCD printer 	| [gpio](https://github.com/remotemcu/remcu-chip-sdks/blob/master/SiLabs/Gecko_SDK/CMakeLists/EFM32TG840F32/test/gpio.c), [lcd](https://github.com/remotemcu/remcu-chip-sdks/blob/master/SiLabs/Gecko_SDK/CMakeLists/EFM32TG840F32/test/lcd.c) 	|  	|
| SAMD20(Microchip) 	| soon 	|  	|  	| [ssd1306](https://github.com/remotemcu/remcu-chip-sdks/tree/master/Atmel/xdk-asf-3.45.0/sam0/CMakeLists/samd20/test/display_ssd1306), [adc](https://github.com/remotemcu/remcu-chip-sdks/tree/master/Atmel/xdk-asf-3.45.0/sam0/CMakeLists/samd20/test/test_adc_light), [dac](https://github.com/remotemcu/remcu-chip-sdks/tree/master/Atmel/xdk-asf-3.45.0/sam0/CMakeLists/samd20/test/test_dac), [gpio](https://github.com/remotemcu/remcu-chip-sdks/tree/master/Atmel/xdk-asf-3.45.0/sam0/CMakeLists/samd20/test/test_gpio)	|  	|
| NRF51422 	|  	|  	|  	| [adc](https://github.com/remotemcu/remcu-chip-sdks/blob/master/nordicsemi/nRF5_SDK_12.3.0_d7731ad/NRF51422/test/main.c) |  	|

### Demo GIFs

<details>
	<summary> <b>(click here to view) </b></summary>

stm32f4_discovery/accell_graph demo:  
![monitor_of_acc](stm32f4_discovery/accell_graph/img/mems_demo.gif)  

stm32f4_discovery/CAN_BUS demo:  
![monitor_of_acc](stm32f4_discovery/CAN_BUS/img/can_bus_demo.gif)

stm32f4_discovery/jupyter-notebook Python notebook demo:  
![slider_f4d](stm32f4_discovery/jupyter-notebook/img/py_demo.gif)  

STM32F767X/jupyter-notebook ADC, DAC, PWM demo:  
![STM32F767X](STM32F767X/img/demo.gif)

STM32F103C8T6-Blue-Pill/Raspberry + Blue Pill CAN bus demo:   
![raspberry can bus](STM32F103C8T6-Blue-Pill/img/CAN_demo.gif)
</details>

## REMCU Lib
The [**REMCU Lib**](https://remotemcu.com/) is a cross-platform library designed for remote access to internal peripherals of various MCU and SoC as if these peripherals were a part of your computer.
The library gives access a user space program to all peripherals of the remote chip  through the same API as peripheral drivers from Software Development Kit (SDK) provided by semiconductor chip vendors or third parties for MCU firmware development.
Thereby a developer doesn’t need to learn new tools but can just reuse his firmware code and examples of the SDK in his code designed for personal or embedded computers.
Firmware and protocol communication development are not required, because the [**REMCU Lib**](https://remotemcu.com/) translates itself the local API function call on computer to the call of the same function of peripheral driver on the remote chip.

Let's have a look at the video below and see what opportunities and applications it provides.

[![REMCU DESCRIPTION](img/preview_description.png)](https://youtu.be/PJPl9Y96hA0)

REMCU Lib Docs: https://remotemcu.com/docs

Full details of the examples can be found in README.md file of the corresponding folders.

## Support policy

If you discover a problem with any of the samples published here that isn't already reported in Issues, open a [New issue](https://github.com/remotemcu/remcu_examples/issues/new?assignees=&labels=bug&template=bug_report.md&title=).

If you have a feature idea - please open a [feature request](https://github.com/remotemcu/remcu_examples/issues/new?assignees=&labels=Feature+request&template=feature_request.md&title=).  
If you have new sample development on the REMCU Lib  - please opening a [pull request](https://github.com/remotemcu/remcu_examples/pulls) or [*"Suggestion of examples"*](https://github.com/remotemcu/remcu_examples/issues/new?assignees=&labels=Suggestion+of+examples&template=suggestion-of-examples.md&title=) request.  
We will be pleased to answer all your questions about the REMCU Lib project through the options/channels below:  
*StackOverflow*. Tag your question with **#remculib** tag. We watch full list of questions and will answer ASAP. Make experience that you've got available for other users!  
[Contact us form](https://remotemcu.com/contact-us)  
[@Email](support@remotemcu.com)  
[Twitter](https://twitter.com/RemoteMcu)  

Thanks in advance!
