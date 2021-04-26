# MCU (ESP32) Lockup Detection Using Internal WatchDog
## I. Introduction
If your program gets stuck for some reason, the fastest way to recover is to restart your computer. One of the most common examples was when the Windows operating system would crash (experience a fatal error) and display the Blue Screen of Death (BSoD), after which the machine would reboot to avoid further damage. Unlike desktop computers, embedded devices must be self-reliant. What if you are unable to restart the computer physically? If the program hangs, it's not always possible to wait for anyone to reset them. Some embedded designs, such as space probes, are simply inaccessible to humans, and a manual reset is not feasible; these could become permanently disabled if they were unable to autonomously recover from faults. In such situations, a human operator's capacity to reset the device will be inadequate to satisfy the product's uptime requirements. As firmware engineers, it is our responsibility to ensure that our devices can always recover from a stuck task or infinite loop. A watchdog is the only way usually employed in cases like these [[1](https://www.microcontrollertips.com/whats-watch-dog-timer-wdt-faq/)], [[2](https://www.embedded.com/introduction-to-watchdog-timers/)]. Watchdog is typically used in embedded systems and other computer-controlled equipment where humans cannot easily access the equipment or would be unable to respond to faults promptly [[3](https://www.researchgate.net/publication/295010877_Architecture_and_Operation_of_a_Watchdog_Timer)].
## II. What is Watchdog Timer?
A **watchdog timer** (WDT) is a hardware timer that can be used to automatically detect software anomalies and automatically generates a system reset if the main program neglects to periodically service it. It is often used to automatically reset an embedded device that hangs because of a software or hardware fault [[4](https://os.mbed.com/cookbook/WatchDog-Timer)]. Generally speaking, a watchdog timer is based on a counter that counts down from some initial value to zero. The embedded software selects the counter's initial value and periodically restarts it. If the counter ever reaches zero before the software restarts it, the software is presumed to be malfunctioning and the processor's reset signal is asserted [[2](https://www.embedded.com/introduction-to-watchdog-timers/)]. The process of restarting the watchdog timer's counter is sometimes called “**kicking the dog**.” The appropriate visual metaphor is that of a man being attacked by a vicious dog. If he keeps kicking the dog, it can't ever bite him. But he must keep kicking the dog at regular intervals to avoid a bite. Similarly, the software must restart the watchdog timer at a regular rate, or risk being restarted. Another term for “*kicking*” is “*feeding*” the watchdog timer. The watchdog timer is continuously fed with the initial value so that the timer value can never reach zero.

| ![space-1.jpg](https://files.readme.io/b302301-out.gif) | 
|:--:| 
| **Fig. 1** *A typical Watchdog setup* |

Fig. 1 shows a typical arrangement of the watchdog timer as an external block to the processor. However, it could also be included within the same chip as the CPU. This is done in many microcontrollers. In either case, the output from the watchdog timer is tied directly to the processor's reset signal. 

**Listing 1** *Basic Watchdog Timer*

In some engineer's opinion that, if an MCU could fail, the same could happen to its internal WDT. Thus, the external WDT was their preferred option. Using an internal WDT requires one less component and is, therefore, cheaper [[6](https://www.eeweb.com/using-the-microcontrollers-watchdog-timer-wdt-effectively/)]. On the other hand, there’s still a valid argument to be made for using an external WDT, because an internal WDT can be turned off accidentally by runaway code. Ultimately, the choice of internal versus external WDT relies on the nature of the application and the production budget for the hardware.

## What are the potential causes of the hang event?
A Microcontroller Unit (MCU) that causes failure can cause a complete standstill in operations. This can inconvenience users or pose functional safety risks in critical applications. A safety mechanism is extremely important. A reliable embedded system requires a combined effort from the hardware designer and the firmware programmer. These are some reasons why MCU may have a failure mechanism [[7](https://resources.altium.com/p/microcontroller-failure-modes-why-they-happen-and-how-to-prevent-them)].
* Electrical noise can cause a power glitch, it may corrupt the program counter.
* Unstable power source.
* Memory stack overflow.
* A hardware component is not responding to request.
* Electrical interference (back-EMF, EMI, electrostatic discharge, cosmic ray, etc.).
* Dead-lock condition.
* Lots of interrupts (ISR).


